#pragma once

#include <iostream>

namespace tree
{
    template <typename Key, typename Compare>
    cartesian<Key, Compare>::cartesian() = default;

    template <typename Key, typename Compare>
    cartesian<Key, Compare>::cartesian(const std::initializer_list<key_type>& data)
    {
        for (const auto& element : data)
        {
            this->insert(element);
        }
    }

    template <typename Key, typename Compare>
    cartesian<Key, Compare>::cartesian(std::initializer_list<key_type>&& data)
    {
        for (auto&& element : data)
        {
            this->insert(std::move(element));
        }
    }

    template <typename Key, typename Compare>
    cartesian<Key, Compare>::cartesian(const cartesian <key_type, key_compare>& other)
    {
        for (const auto& element : other)
        {
            this->insert(element);
        }
    }

    template <typename Key, typename Compare>
    cartesian<Key, Compare>::cartesian(cartesian <key_type, key_compare>&& other) noexcept
    {
        std::swap(this->head, other.head);
        std::swap(this->m_size, other.m_size);
    }

    template <typename Key, typename Compare>
    cartesian<Key, Compare>::~cartesian()
    {
        this->clear();
    }

    template <typename Key, typename Compare>
    cartesian <Key, Compare>& cartesian<Key, Compare>::operator=(const cartesian <key_type, key_compare>& other)
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
    cartesian <Key, Compare>& cartesian<Key, Compare>::operator=(cartesian <key_type, key_compare>&& other) noexcept
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
    typename cartesian<Key, Compare>::iterator cartesian<Key, Compare>::begin()
    {
        return iterator(head);
    }

    template <typename Key, typename Compare>
    typename cartesian<Key, Compare>::const_iterator cartesian<Key, Compare>::begin() const
    {
        return const_iterator(head);
    }

    template <typename Key, typename Compare>
    typename cartesian<Key, Compare>::const_iterator cartesian<Key, Compare>::cbegin() const
    {
        return const_iterator(head);
    }

    template <typename Key, typename Compare>
    typename cartesian<Key, Compare>::iterator cartesian<Key, Compare>::end()
    {
        return iterator(head, std::make_optional<node_ptr>(nullptr));
    }

    template <typename Key, typename Compare>
    typename cartesian<Key, Compare>::const_iterator cartesian<Key, Compare>::end() const
    {
        return const_iterator(head, std::make_optional<node_ptr>(nullptr));
    }

    template <typename Key, typename Compare>
    typename cartesian<Key, Compare>::const_iterator cartesian<Key, Compare>::cend() const
    {
        return const_iterator(std::make_optional<node_ptr>(nullptr));
    }

    //////////////////
    //   CAPACITY   //
    //////////////////

    template <typename Key, typename Compare>
    bool cartesian<Key, Compare>::empty() const noexcept
    {
        return size() == 0;
    }

    template <typename Key, typename Compare>
    std::size_t cartesian<Key, Compare>::size() const noexcept
    {
        return m_size;
    }

    ///////////////////
    //   MODIFIERS   //
    ///////////////////

    template <typename Key, typename Compare>
    void cartesian<Key, Compare>::clear() noexcept
    {
        while (!this->empty())
        {
            erase(this->head->value);
        }
    }

    template <typename Key, typename Compare>
    typename cartesian<Key, Compare>::node_ptr cartesian<Key, Compare>::insert(key_type key)
    {
#if CARTESIAN_TREE_DEBUG_INSERT == 1
        std::cerr << "insert" << std::endl;
#endif

        // step 1: create a new node with the given key and a random priority
         const auto priority = get_random_priority();
        auto child = new node_type(key, priority);

        // step 2: split the initial tree by the key and merge in the new node
        auto [lhs, rhs] = this->split(key);
        auto result = this->merge(lhs, child);
        result = this->merge(result, rhs);

        head = result;
        m_size++;
        return child;
    }

    template <typename Key, typename Compare>
    void cartesian<Key, Compare>::erase(const key_type& key)
    {
        // step 1: split by key
        auto [lhs, rhs] = this->split(key);
        // lhs keys: <= key
        // rhs keys: > key

        // step 2: node with the key is the rightmost node in lhs,
        //         replace it with it's left child

        node_ptr parent = nullptr;
        node_ptr to_delete = lhs;

        while (to_delete->value != key)
        {
            parent = to_delete;
            to_delete = to_delete->right;
        }

        if (parent != nullptr)
        {
            parent->right = to_delete->left;
        }

        delete to_delete;
        m_size--;

        // step 3: merge back
        this->head = merge(lhs, rhs);
    }

    template <typename Key, typename Compare>
    [[nodiscard]] std::pair<typename cartesian<Key, Compare>::node_ptr,
                            typename cartesian<Key, Compare>::node_ptr>
    cartesian<Key, Compare>::split(const key_type& key)
    {
        // TODO: reconsider

        if (head == nullptr)
        {
            return {nullptr, nullptr};
        }

        auto root = head;
        if (root->value == key)
        {
            return {nullptr, root};
        }

        node_ptr previous_node;
        auto current_node = head;
        auto current_key = current_node->value;
        bool first_cmp = key_cmp(current_key, key);
        bool last_cmp;

        while (current_node != nullptr)
        {
            previous_node = current_node;
            current_key = current_node->value;

            if (key_cmp(key, current_key))
            {
                last_cmp = false;
                current_node = current_node->left;
            }
            else if (key_cmp(current_key, key))
            {
                last_cmp = true;
                current_node = current_node->right;
            }
            else
            {
                break;
            }
        }

        if (last_cmp)
        {
            previous_node->right = nullptr;
        }
        else
        {
            previous_node->left = nullptr;
        }

        if (first_cmp)
        {
            return {root, current_node};
        }
        else
        {
            return {current_node, root};
        }
    }

    template <typename Key, typename Compare>
    [[nodiscard]]  typename cartesian<Key, Compare>::node_ptr
    cartesian<Key, Compare>::merge(node_ptr node_lhs, node_ptr& node_rhs)
    {
        if (node_lhs == nullptr)
        {
            return node_rhs;
        }

        if (node_rhs == nullptr)
        {
            return node_lhs;
        }

        auto priority_lhs = node_lhs->priority;
        auto priority_rhs = node_rhs->priority;
        node_ptr root = (priority_lhs >= priority_rhs) ? node_lhs : node_rhs;

        while (true)
        {
            if (key_cmp(node_rhs->value, node_lhs->value))
            {
                std::swap(node_lhs, node_rhs);
                std::swap(priority_lhs, priority_rhs);
            }

            if (priority_lhs >= priority_rhs)
            {
                auto right_child_lhs = node_lhs->right;
                if (right_child_lhs != nullptr)
                {
                    node_lhs = right_child_lhs;
                    priority_lhs = node_lhs->priority;
                }
                else
                {
                    node_lhs->right = node_rhs;
                    break;
                }
            }
            else
            {
                auto left_child_rhs = node_rhs->left;
                if (left_child_rhs != nullptr)
                {
                    node_rhs = left_child_rhs;
                    priority_rhs = node_rhs->priority;
                }
                else
                {
                    node_rhs->left = node_lhs;
                    break;
                }
            }
        }

        return root;
    }

    /////////////////
    //   LOOK UP   //
    /////////////////

    template <typename Key, typename Compare>
    typename cartesian<Key, Compare>::iterator cartesian<Key, Compare>::find(const key_type& value)
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
    typename cartesian<Key, Compare>::const_iterator cartesian<Key, Compare>::find(const key_type& value) const
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
    [[nodiscard]] bool cartesian<Key, Compare>::is_ordered(node_ptr subtree) const noexcept
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

    template <typename Key, typename Compare>
    bool cartesian<Key, Compare>::is_heap(node_ptr subtree) const noexcept
    {
        if (subtree == nullptr)
        {
            return true;
        }

        key_type priority = subtree->priority;

        node_ptr subtree_lhs = subtree->left;
        bool is_lhs_heap =
                subtree_lhs == nullptr ||
                key_cmp(subtree_lhs->priority, priority) && is_heap(subtree_lhs);

        node_ptr subtree_rhs = subtree->right;
        bool is_rhs_heap =
                subtree_rhs == nullptr ||
                key_cmp(subtree_rhs->priority, priority) && is_heap(subtree_lhs);

        return is_lhs_heap && is_rhs_heap;
    }

    template <typename Key, typename Compare>
    [[nodiscard]] bool cartesian<Key, Compare>::is_cartesian(node_ptr subtree) const noexcept
    {
        if (subtree == nullptr)
        {
            subtree = head;
        }

        return is_ordered(subtree) && is_heap(subtree);
    }

    template <typename Key, typename Compare>
    auto cartesian<Key, Compare>::get_random_priority() const
    {
        return priority_dist(gen);
    }

} // namespace tree