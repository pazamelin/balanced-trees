#pragma once

#include <iostream>

namespace tree
{
    template <typename Key, typename Compare>
    avl<Key, Compare>::avl() = default;

    template <typename Key, typename Compare>
    avl<Key, Compare>::avl(const std::initializer_list<key_type>& data)
    {
        for (const auto& element : data)
        {
            this->insert(element);
        }
    }

    template <typename Key, typename Compare>
    avl<Key, Compare>::avl(std::initializer_list<key_type>&& data)
    {
        for (auto&& element : data)
        {
            this->insert(std::move(element));
        }
    }

    template <typename Key, typename Compare>
    avl<Key, Compare>::avl(const avl <key_type, key_compare>& other)
    {
        for (const auto& element : other)
        {
            this->insert(element);
        }
    }

    template <typename Key, typename Compare>
    avl<Key, Compare>::avl(avl <key_type, key_compare>&& other) noexcept
    {
        std::swap(this->head, other.head);
        std::swap(this->m_size, other.m_size);
    }

    template <typename Key, typename Compare>
    avl<Key, Compare>::~avl()
    {
        this->clear();
    }

    template <typename Key, typename Compare>
    avl <Key, Compare>& avl<Key, Compare>::operator=(const avl <key_type, key_compare>& other)
    {
        if (this != &other)
        {
            for (const auto& element : other)
            {
                this->insert(element);
            }
        }
        return *this;
    }

    template <typename Key, typename Compare>
    avl <Key, Compare>& avl<Key, Compare>::operator=(avl <key_type, key_compare>&& other) noexcept
    {
        if (this != &other)
        {
            for (const auto& element : other)
            {
                this->insert(std::move(element));
            }
        }
        return *this;
    }

    ///////////////////
    //   ITERATORS   //
    ///////////////////

    template <typename Key, typename Compare>
    typename avl<Key, Compare>::iterator avl<Key, Compare>::begin()
    {
        return iterator(head);
    }

    template <typename Key, typename Compare>
    typename avl<Key, Compare>::const_iterator avl<Key, Compare>::begin() const
    {
        return const_iterator(head);
    }

    template <typename Key, typename Compare>
    typename avl<Key, Compare>::const_iterator avl<Key, Compare>::cbegin() const
    {
        return const_iterator(head);
    }

    template <typename Key, typename Compare>
    typename avl<Key, Compare>::iterator avl<Key, Compare>::end()
    {
        return iterator(head, std::make_optional<node_ptr>(nullptr));
    }

    template <typename Key, typename Compare>
    typename avl<Key, Compare>::const_iterator avl<Key, Compare>::end() const
    {
        return const_iterator(head, std::make_optional<node_ptr>(nullptr));
    }

    template <typename Key, typename Compare>
    typename avl<Key, Compare>::const_iterator avl<Key, Compare>::cend() const
    {
        return const_iterator(std::make_optional<node_ptr>(nullptr));
    }

    //////////////////
    //   CAPACITY   //
    //////////////////

    template <typename Key, typename Compare>
    bool avl<Key, Compare>::empty() const noexcept
    {
        return size() == 0;
    }

    template <typename Key, typename Compare>
    std::size_t avl<Key, Compare>::size() const noexcept
    {
        return m_size;
    }

    ///////////////////
    //   MODIFIERS   //
    ///////////////////

    template <typename Key, typename Compare>
    void avl<Key, Compare>::clear() noexcept
    {
        while (!this->empty())
        {
            erase(this->head->value);
        }
    }

    template <typename Key, typename Compare>
    typename avl<Key, Compare>::node_ptr avl<Key, Compare>::insert(key_type key)
    {
#if AVL_TREE_DEBUG_INSERT == 1
        std::cerr << "insert" << std::endl;
#endif

        clear_cache();

        node_ptr child = nullptr;

        if (head == nullptr)
        {
            head = new node_type(key);
        }
        else
        {
            node_ptr parent = find_place(key);
            if (parent->value == key)
            {
                return parent;
            }

            auto is_right_child = static_cast<bool>(cmp_cache.back());
            if (!is_right_child)
            {
                child = new node_type{key};
                parent->left = child;
            }
            else
            {
                child = new node_type{key};
                parent->right = child;
            }

            if (branch_root == nullptr)
            {
                update_balance_factors(head);
                head = rebalance(head);
            }
            else
            {
                update_balance_factors(branch_root);
                branch_root = rebalance(branch_root);
                if (is_branch_right_child)
                {
                    joint_to_branch->right = branch_root;
                }
                else
                {
                    joint_to_branch->left = branch_root;
                }
            }

            clear_cache();
        }

        m_size++;
        return child;
    }

    template <typename Key, typename Compare>
    void avl<Key, Compare>::erase(const key_type& key)
    {
        clear_cache();

        // caching until the key
        node_ptr node = find_place(key, false);
        if (node == nullptr)
        {   // no such key found
            return;
        }

        if (m_size == 1)
        {   // it's the onliest node
            m_size--;
            delete head;
            head = nullptr;
            return;
        }

        // found the node in the tree, cached it
        path_cache.pop(); // pop this node from cache
        node_ptr left_child = node->left;
        node_ptr right_child = node->right;

        if (right_child == nullptr)
        {
#if AVL_TREE_DEBUG_ERASE == 1
            std::cerr << "erase, case 1 " << std::endl;
#endif

            // Case 1 in AVL deletion -
            // the node has a left child, but no right one
            // replace the node with it's left child

            node_ptr parent = path_cache.empty() ? nullptr : path_cache.top();
            if (parent != nullptr)
            {
                if (cmp_cache.back())
                {
                    parent->right = left_child;
                }
                else
                {
                    parent->left = left_child;
                }
            }
            else
            {
                // the node is root
                head = left_child;
            }

            if (left_child != nullptr)
            {
                left_child->balance = node->balance;

                // cache the replacement so it might be rebalanced later
                path_cache.push(left_child);
                cmp_cache.push_back(false);
            }

            delete node;
        }
        else
        {
            if (right_child->left == nullptr)
            {
#if AVL_TREE_DEBUG_ERASE == 1
                std::cerr << "erase, case 2 " << std::endl;
#endif

                // Case 2 in AVL deletion -
                // the node has both left and right children, but
                // the right child has no left child -
                // replace the node with it's right child and give it node's left child

                node_ptr parent = path_cache.empty() ? nullptr : path_cache.top();
                if (parent != nullptr)
                {
                    if (cmp_cache.back())
                    {
                        parent->right = right_child;
                    }
                    else
                    {
                        parent->left = right_child;
                    }
                }
                else
                {
                    // the node is root
                    head = right_child;
                }

                right_child->left = left_child;
                right_child->balance = node->balance;

                // cache the replacement so it might be rebalanced later
                path_cache.push(right_child);
                cmp_cache.push_back(true);

                delete node;
            }
            else
            {
#if AVL_TREE_DEBUG_ERASE == 1
                std::cerr << "erase, case 3 " << std::endl;
#endif

                // Case 3 in AVL deletion -
                // the node has a left child and a right one with left child

                // find inorder successor of the node and cache the path
                std::queue<node_ptr> path_cache_to_next;
                node_ptr next = right_child;
                while (true)
                {
                    if (next->left != nullptr)
                    {
                        path_cache_to_next.push(next);
                        next = next->left;
                    }
                    else
                    {
                        break;
                    }
                }

                // replace the node with the successor found
                next->left = node->left;
                path_cache_to_next.back()->left = next->right;
                next->right = node->right;
                next->balance = node->balance;

                node_ptr parent = path_cache.empty() ? nullptr : path_cache.top();
                if (parent != nullptr)
                {
                    if (cmp_cache.back())
                    {
                        parent->right = next;
                    }
                    else
                    {
                        parent->left = next;
                    }
                }
                else
                {
                    // the node is root
                    head = next;
                }

                // merging caches properly, so the successor comes before
                // the recently cached path to it
                cmp_cache.push_back(true);
                path_cache.push(next);
                while (!path_cache_to_next.empty())
                {
                    path_cache.push(path_cache_to_next.front());
                    cmp_cache.push_back(false);
                    path_cache_to_next.pop();
                }

                delete node;
            }
        }

        // update balances and rebalance
        while (!path_cache.empty())
        {
            node_ptr upd_node = path_cache.top();
            path_cache.pop();
            node_ptr parent_node = path_cache.empty() ? nullptr : path_cache.top();

            if (!cmp_cache.back())
            {
                cmp_cache.pop_back();

                // Update upd_node’s balance factor after left-side AVL deletion
                upd_node->balance = detail::shift_right(upd_node->balance);
                if (upd_node->balance == detail::balance_factor::rhs_1)
                {
                    break;
                }
                else if (upd_node->balance == detail::balance_factor::rhs_2)
                {
                    node_ptr upd_right_child = upd_node->right;
                    if (upd_right_child->balance == detail::balance_factor::lhs_1)
                    {
                        upd_node = rotate_right_left(upd_node);

                        if (parent_node != nullptr)
                        {
                            if (cmp_cache.back())
                            {
                                parent_node->right = upd_node;
                            }
                            else
                            {
                                parent_node->left = upd_node;
                            }
                        }
                        else
                        {
                            head = upd_node;
                        }
                    }
                    else
                    {
                        upd_node->right = upd_right_child->left;
                        upd_right_child->left = upd_node;

                        if (parent_node != nullptr)
                        {
                            if (cmp_cache.back())
                            {
                                parent_node->right = upd_right_child;
                            }
                            else
                            {
                                parent_node->left = upd_right_child;
                            }
                        }
                        else
                        {
                            head = upd_right_child;
                        }

                        if (upd_right_child->balance == detail::balance_factor::zero)
                        {
                            upd_right_child->balance = detail::balance_factor::lhs_1;
                            upd_node->balance = detail::balance_factor::rhs_1;
                            break;
                        }
                        else
                        {
                            upd_right_child->balance = detail::balance_factor::zero;
                            upd_node->balance = detail::balance_factor::zero;
                        }
                    }
                }
            }
            else
            {
                cmp_cache.pop_back();

                // Update upd_node’s balance factor after right-side AVL deletion
                upd_node->balance = detail::shift_left(upd_node->balance);
                if (upd_node->balance == detail::balance_factor::lhs_1)
                {
                    break;
                }
                else if (upd_node->balance == detail::balance_factor::lhs_2)
                {
                    node_ptr upd_left_child = upd_node->left;
                    if (upd_left_child->balance == detail::balance_factor::rhs_1)
                    {
                        upd_node = rotate_left_right(upd_node);

                        if (parent_node != nullptr)
                        {
                            if (cmp_cache.back())
                            {
                                parent_node->right = upd_node;
                            }
                            else
                            {
                                parent_node->left = upd_node;
                            }
                        }
                        else
                        {
                            head = upd_node;
                        }
                    }
                    else
                    {
                        upd_node->left = upd_left_child->right;
                        upd_left_child->right = upd_node;
                        if (parent_node != nullptr)
                        {
                            if (cmp_cache.back())
                            {
                                parent_node->right = upd_left_child;
                            }
                            else
                            {
                                parent_node->left = upd_left_child;
                            }
                        }
                        else
                        {
                            head = upd_left_child;
                        }

                        if (upd_left_child->balance == detail::balance_factor::zero)
                        {
                            upd_left_child->balance = detail::balance_factor::rhs_1;
                            upd_node->balance = detail::balance_factor::lhs_1;
                            break;
                        }
                        else
                        {
                            upd_left_child->balance = detail::balance_factor::zero;
                            upd_node->balance = detail::balance_factor::zero;
                        }
                    }
                }
            }
        }

        m_size--;
    }

    /////////////////
    //   LOOK UP   //
    /////////////////

    template <typename Key, typename Compare>
    typename avl<Key, Compare>::iterator avl<Key, Compare>::find(const key_type& value)
    {
        node_ptr current = head;
        while (current != nullptr)
        {
            if (key_cmp(value, current->value))
            {
                current = current->left;
            }
            else if (key_cmp(current->value, value))
            {
                current = current->right;
            }
            else
            {
                break;
            }
        }

        return iterator(head, current);
    }

    template <typename Key, typename Compare>
    typename avl<Key, Compare>::const_iterator avl<Key, Compare>::find(const key_type& value) const
    {
        node_ptr current = head;
        while (current != nullptr)
        {
            if (key_cmp(value, current->value))
            {
                current = current->left;
            }
            else if (key_cmp(current->value, value))
            {
                current = current->right;
            }
            else
            {
                break;
            }
        }

        return const_iterator(head, current);
    }


    template <typename Key, typename Compare>
    [[nodiscard]] bool avl<Key, Compare>::is_avl(node_ptr subtree) const noexcept
    {
        if (subtree == nullptr)
        {
            subtree = head;
        }

        return is_ordered(subtree) && is_balanced(subtree) && check_balance_factors(subtree).first;
    }

    template <typename Key, typename Compare>
    std::pair<bool, int>
    avl<Key, Compare>::check_balance_factors(node_ptr subtree) const noexcept
    {
        if (subtree == nullptr)
        {
            return {true, -1};
        }

        const auto[is_left_good, left_height] = check_balance_factors(subtree->left);
        const auto[is_right_good, right_height] = check_balance_factors(subtree->right);

        int height = 1 + std::max(left_height, right_height);
        bool is_good = true;

        if (!(is_left_good && is_right_good) ||
            subtree->balance != detail::balance_factor(right_height - left_height))
        {
            is_good = false;
        }

        return {is_good, height};
    }

    template <typename Key, typename Compare>
    [[nodiscard]] bool avl<Key, Compare>::is_balanced(node_ptr subtree) const noexcept
    {
        if (subtree == nullptr)
        {
            return true;
        }

        if (subtree->balance == detail::balance_factor::lhs_2 || 
            subtree->balance == detail::balance_factor::rhs_2)
        {
            return false;
        }


        return is_balanced(subtree->left) && is_balanced(subtree->right);
    }

    template <typename Key, typename Compare>
    [[nodiscard]] bool avl<Key, Compare>::is_ordered(node_ptr subtree) const noexcept
    {
        if (subtree == nullptr)
        {
            return true;
        }

        key_type key = subtree->value;

        node_ptr subtree_lhs = subtree->left;
        bool is_lhs_ordered =
                subtree_lhs == nullptr || 
                key_cmp(subtree_lhs->value, key) && is_ordered(subtree_lhs);

        node_ptr subtree_rhs = subtree->right;
        bool is_rhs_ordered =
                subtree_rhs == nullptr || 
                key_cmp(key, subtree_rhs->value) && is_ordered(subtree_lhs);

        return is_lhs_ordered && is_rhs_ordered;
    }

    ///////////////////
    //   BALANCING   //
    ///////////////////

    template <typename Key, typename Compare>
    typename avl<Key, Compare>::node_ptr
    avl<Key, Compare>::find_place(const key_type& value, bool last_nonzero) const
    {
        cmp_cache.clear();
        joint_to_branch = nullptr;
        branch_root = nullptr;

        node_ptr previous = nullptr;
        node_ptr current = head;

        while (current != nullptr)
        {
            previous = current;
            path_cache.push(previous);

            if (key_cmp(value, current->value))
            {
                current = current->left;
                cmp_cache.push_back(false);
            }
            else if (key_cmp(current->value, value))
            {
                current = current->right;
                cmp_cache.push_back(true);
            }
            else
            {
                break;
            }

            if (last_nonzero and
                current != nullptr and
                current->balance != detail::balance_factor::zero)
            {
                joint_to_branch = previous;
                is_branch_right_child = cmp_cache.back();
                branch_root = current;
                cmp_cache.clear();
            }
        }

        return last_nonzero ? previous : current;
    }

    template <typename Key, typename Compare>
    void avl<Key, Compare>::update_balance_factors(node_ptr subtree)
    {
        for (auto cmp_result : cmp_cache)
        {
            if (!cmp_result)
            {
                subtree->balance = detail::shift_left(subtree->balance);
                subtree = subtree->left;
            }
            else
            {
                subtree->balance = detail::shift_right(subtree->balance);
                subtree = subtree->right;
            }

            if (subtree == nullptr)
            {
                throw std::runtime_error("failed to update balances, unexpected nullptr");
            }
        }
    }

    template <typename Key, typename Compare>
    typename avl<Key, Compare>::node_ptr
    avl<Key, Compare>::rotate_right(node_ptr subtree) noexcept
    {
#if AVL_TREE_DEBUG_ROTATIONS == 1
        std::cerr << "right rotation on " << subtree->value << std::endl;
#endif

        node_ptr root = subtree->left;
        subtree->left = root->right;
        root->right = subtree;
        return root;
    }


    template <typename Key, typename Compare>
    typename avl<Key, Compare>::node_ptr
    avl<Key, Compare>::rotate_left(node_ptr subtree) noexcept
    {
#if AVL_TREE_DEBUG_ROTATIONS == 1
        std::cerr << "left rotation on " << subtree->value << std::endl;
#endif

        node_ptr root = subtree->right;
        subtree->right = root->left;
        root->left = subtree;
        return root;
    }

    template <typename Key, typename Compare>
    typename avl<Key, Compare>::node_ptr
    avl<Key, Compare>::rotate_left_right(node_ptr subtree)
    {
#if AVL_TREE_DEBUG_ROTATIONS == 1
        std::cerr << "left-right rotation: " << std::endl;
#endif

        using detail::balance_factor;

        subtree->left = rotate_left(subtree->left);
        subtree = rotate_right(subtree);

        switch (subtree->balance)
        {
            case balance_factor::lhs_1:
            {
                subtree->left->balance = balance_factor::zero;
                subtree->right->balance = balance_factor::rhs_1;
                break;
            }
            case balance_factor::zero:
            {
                subtree->left->balance = balance_factor::zero;
                subtree->right->balance = balance_factor::zero;
                break;
            }
            case balance_factor::rhs_1:
            {
                subtree->left->balance = balance_factor::lhs_1;
                subtree->right->balance = balance_factor::zero;
                break;
            }
            default:
            {
                throw std::runtime_error("failed to rebalance, "
                                         "unexpected balance factor");
            }
        }

        subtree->balance = balance_factor::zero;
        return subtree;
    }

    template <typename Key, typename Compare>
    typename avl<Key, Compare>::node_ptr
    avl<Key, Compare>::rotate_right_left(node_ptr subtree)
    {
#if AVL_TREE_DEBUG_ROTATIONS == 1
        std::cerr << "right-left rotation: " << std::endl;
#endif

        using detail::balance_factor;

        subtree->right = rotate_right(subtree->right);
        subtree = rotate_left(subtree);

        switch (subtree->balance)
        {
            case balance_factor::lhs_1:
            {
                subtree->left->balance = balance_factor::zero;
                subtree->right->balance = balance_factor::rhs_1;
                break;
            }
            case balance_factor::zero:
            {
                subtree->left->balance = balance_factor::zero;
                subtree->right->balance = balance_factor::zero;
                break;
            }
            case balance_factor::rhs_1:
            {
                subtree->left->balance = balance_factor::lhs_1;
                subtree->right->balance = balance_factor::zero;
                break;
            }
            default:
            {
                throw std::runtime_error("failed to rebalance, "
                                         "unexpected balance factor");
            }
        }

        subtree->balance = balance_factor::zero;
        return subtree;
    }

    template <typename Key, typename Compare>
    typename avl<Key, Compare>::node_ptr avl<Key, Compare>::rebalance(node_ptr subtree)
    {
        using detail::balance_factor;

        if (subtree->balance == balance_factor::lhs_2)
        {
            // rebalance avl-tree after insertion in left subtree
            if (subtree->left->balance == balance_factor::lhs_1)
            {
                subtree = rotate_right(subtree);
                subtree->balance = balance_factor::zero;
                subtree->right->balance = balance_factor::zero;
            }
            else if (subtree->left->balance == balance_factor::rhs_1)
            {
                // LEFT-RIGHT
                subtree = rotate_left_right(subtree);
            }
        }
        else if (subtree->balance == balance_factor::rhs_2)
        {
            // rebalance avl-tree after insertion in right subtree
            if (subtree->right->balance == balance_factor::rhs_1)
            {
                subtree = rotate_left(subtree);
                subtree->balance = balance_factor::zero;
                subtree->left->balance = balance_factor::zero;
            }
            else if (subtree->right->balance == balance_factor::lhs_1)
            {
                // RIGHT-LEFT
                subtree = rotate_right_left(subtree);
            }
        }

        return subtree;
    }

    template <typename Key, typename Compare>
    void avl<Key, Compare>::clear_cache() const
    {
        cmp_cache.clear();
        while (!path_cache.empty())
        {
            path_cache.pop();
        }

        joint_to_branch = nullptr;
        branch_root = nullptr;
    }

} // namespace tree