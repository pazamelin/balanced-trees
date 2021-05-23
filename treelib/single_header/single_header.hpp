#pragma once

#include <exception>

namespace tree::detail
{
    ///////////////////
    //   BASE NODE   //
    ///////////////////

    template <typename ValueType>
    struct Node
    {
		using value_type = ValueType;
        explicit Node(ValueType key) : value{key} { }

        void set_left(Node* subtree) noexcept
        {
            this->left = subtree;
        }

        void set_right(Node* subtree) noexcept
        {
            this->right = subtree;
        }

        ValueType value = ValueType();
        Node* left   = nullptr;
        Node* right  = nullptr;
    };

    ///////////////////
    //   AVL NODE   //
    ///////////////////

    enum class balance_factor : char {lhs_2 = -2, lhs_1 = -1, zero = 0, rhs_1 = 1, rhs_2 = 2};

    constexpr balance_factor shift_left(balance_factor balance)
    {
        switch (balance)
        {
            case balance_factor::lhs_2:
                throw std::length_error("balance factor cannot be less than lhs_2");
            case balance_factor::lhs_1:
                return balance_factor::lhs_2;
            case balance_factor::zero:
                return balance_factor::lhs_1;
            case balance_factor::rhs_1:
                return balance_factor::zero;
            case balance_factor::rhs_2:
                return balance_factor::rhs_1;
            default:
                throw std::length_error("unknown balance factor");
        }
    }

    constexpr balance_factor shift_right(balance_factor balance)
    {
        switch (balance)
        {
            case balance_factor::lhs_2:
                return balance_factor::lhs_1;
            case balance_factor::lhs_1:
                return balance_factor::zero;
            case balance_factor::zero:
                return balance_factor::rhs_1;
            case balance_factor::rhs_1:
                return balance_factor::rhs_2;
            case balance_factor::rhs_2:
                throw std::length_error("balance factor cannot be greater than rhs_2");
            default:
                throw std::length_error("unknown balance factor");
        }
    }

    template <typename ValueType>
    struct NodeAVL
    {
        explicit NodeAVL(ValueType value) : value{std::move(value)} { }

        void set_left(NodeAVL<ValueType>* subtree) noexcept
        {
            this->left = subtree;
        }

        void set_right(NodeAVL<ValueType>* subtree) noexcept
        {
            this->right = subtree;
        }

        ValueType value = ValueType();
        NodeAVL* left   = nullptr;
        NodeAVL* right  = nullptr;

        balance_factor balance = balance_factor::zero;

        using value_type = ValueType;
    };

    ///////////////////
    //   AVL NODE   //
    ///////////////////

    template <typename ValueType>
    struct NodeCartesian
    {
        explicit NodeCartesian(ValueType value, int priority)
            : value{std::move(value)},
              priority{priority}
        { }

        void set_left(NodeAVL<ValueType>* subtree) noexcept
        {
            this->left = subtree;
        }

        void set_right(NodeAVL<ValueType>* subtree) noexcept
        {
            this->right = subtree;
        }

        ValueType value = ValueType();
        NodeCartesian* left   = nullptr;
        NodeCartesian* right  = nullptr;
        int priority = 0;

        using value_type = ValueType;
    };

}
#pragma once

#include <stack>
#include <optional>

// #include "detail/node.hpp"

namespace tree
{
    template <typename Node>
    class NodeIterator : public std::iterator<std::bidirectional_iterator_tag, Node>
    {
    public:
        using value_type = typename Node::value_type;
        using self_type = NodeIterator<Node>;

    public:
        explicit NodeIterator(Node* head, std::optional<Node*> until = {});

        value_type& operator * ();
        const value_type& operator * () const;

        self_type& operator ++ ();
        self_type operator ++ (int);

        self_type& operator -- ();
        self_type operator -- (int);

        bool operator == (const NodeIterator<Node>& other);
        bool operator != (const NodeIterator<Node>& other);

        Node* get_ptr();
        const Node* get_ptr() const;

    private:
        void to_leftest(Node* node);
        void to_rightest(Node* node);

        std::stack<Node*> node_stack;
    };

} // namespace tree

// #include "detail/iterator.tpp"
#pragma once

#include <iostream>

namespace tree
{
    template <typename Node>
    NodeIterator<Node>::NodeIterator(Node* head, std::optional<Node*> until)
    {
        if (head == nullptr)
        {
            node_stack.push(nullptr);
            return;
        }

        if (until.has_value())
        {
            if (until.value() != nullptr)
            {
                const Node* const until_ptr = until.value();
                const auto& until_key = until_ptr->value;

                Node* current = head;
                while (current != nullptr)
                {
                    node_stack.push(current);

                    if (current == until_ptr)
                    {
                        break;
                    }

                    if (until_key < current->value)
                    {
                        current = current->left;
                    }
                    else
                    {
                        current = current->right;
                    }
                }
            }
            else
            {
                // end()
                to_rightest(head);
                node_stack.push(nullptr);
            }
        }
        else
        {
            // begin()
            to_leftest(head);
        }
    }

    template <typename Node>
    typename Node::value_type& NodeIterator<Node>::operator * ()
    {
        return node_stack.top()->value;
    }

    template <typename Node>
    const typename Node::value_type& NodeIterator<Node>::operator * () const
    {
        return node_stack.top()->value;
    }

    template <typename Node>
    NodeIterator<Node>& NodeIterator<Node>::operator ++ ()
    {
        Node* current = node_stack.top();
        if (current == nullptr)
        {
            return *this;
        }

        else if (current->right != nullptr)
        {
            // if there is right subtree - go to the smallest node in it
            current = current->right;

            to_leftest(current);
        }
        else
        {
            // if there is no right child -
            // go up to the closest ancestor with current node in it's left subtree
            node_stack.pop();
            if (!node_stack.empty())
            {
                Node *temp = node_stack.top();
                while (temp && current == temp->right)
                {
                    current = temp;
                    node_stack.pop();
                    temp = node_stack.empty() ? nullptr : node_stack.top();
                }
            }
        }

        if (node_stack.empty())
        {
            // end()
            to_rightest(current);
            node_stack.push(nullptr);
        }

        return *this;
    }

    template <typename Node>
    NodeIterator<Node> NodeIterator<Node>::operator ++ (int)
    {
        auto temp = *this;
        ++*this;
        return temp;
    }

    template <typename Node>
    NodeIterator<Node>& NodeIterator<Node>::operator -- ()
    {
        Node* current = node_stack.top();
        if (current == nullptr)
        {   // prev from end()
            node_stack.pop();
            current = node_stack.top();
        }

        else if (current->left != nullptr)
        {
            // if there is left subtree - go to the biggest node in it
            current = current->left;

            to_rightest(current);
        }
        else
        {
            // if there is no left child -
            // go up to the closest ancestor with current node in it's right subtree
            node_stack.pop();
            Node *temp = node_stack.top();

            while (temp && current == temp->left)
            {
                current = temp;
                node_stack.pop();
                temp = node_stack.empty() ? nullptr : node_stack.top();
            }
        }

        if (node_stack.empty())
        {
            // end()
            to_rightest(current);
            node_stack.push(nullptr);
        }

        return *this;
    }

    template <typename Node>
    NodeIterator<Node> NodeIterator<Node>::operator -- (int)
    {
        auto temp = *this;
        --*this;
        return temp;
    }

    template <typename Node>
    bool NodeIterator<Node>::operator == (const NodeIterator<Node>& other)
    {
        if (this->node_stack.empty())
        {
            if (other.node_stack.empty())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (other.node_stack.empty())
            {
                return false;
            }
            else
            {
                return this->node_stack.top() == other.node_stack.top();
            }
        }
    }

    template <typename Node>
    bool NodeIterator<Node>::operator != (const NodeIterator<Node>& other)
    {
        return !(*this == other);
    }

    template <typename Node>
    Node* NodeIterator<Node>::get_ptr()
    {
        return node_stack.top();
    }

    template <typename Node>
    const Node* NodeIterator<Node>::get_ptr() const
    {
        return node_stack.top();
    }

    template <typename Node>
    void NodeIterator<Node>::to_leftest(Node* node)
    {
        while (node != nullptr)
        {
            node_stack.push(node);
            node = node->left;
        }
    }

    template <typename Node>
    void NodeIterator<Node>::to_rightest(Node* node)
    {
        while (node != nullptr)
        {
            node_stack.push(node);
            node = node->right;
        }
    }
}
#pragma once

#include <iterator>
#include <cstddef>
#include <utility>
#include <vector>
#include <exception>
#include <optional>
#include <initializer_list>
#include <stack>
#include <queue>

// #include "detail/node.hpp"
// #include "iterator.hpp"

#define AVL_TREE_DEBUG_ROTATIONS 0
#define AVL_TREE_DEBUG_INSERT 0
#define AVL_TREE_DEBUG_ERASE 0

namespace tree
{
    template <typename Key, typename Compare = std::less<Key>>
    class avl
    {
    public:
        using key_type = Key;
        using key_compare = Compare;
        using node_type = tree::detail::NodeAVL<key_type>;
        using node_ptr = node_type*;
        using iterator = tree::NodeIterator<node_type>;
        using const_iterator = tree::NodeIterator<const node_type>;

    public:
        avl();

        avl(const std::initializer_list<key_type>& data);
        avl(std::initializer_list<key_type>&& data);

        avl(const avl<key_type, key_compare>& other);
        avl(avl<key_type, key_compare>&& other) noexcept;

        ~avl();

        avl<key_type, key_compare>& operator = (const avl<key_type, key_compare>& other);
        avl<key_type, key_compare>& operator = (avl<key_type, key_compare>&& other) noexcept;

        ///////////////////
        //   ITERATORS   //
        ///////////////////

        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;

        iterator end();
        const_iterator end() const;
        const_iterator cend() const;

        //////////////////
        //   CAPACITY   //
        //////////////////

        bool empty() const noexcept;
        std::size_t size() const noexcept;

        ///////////////////
        //   MODIFIERS   //
        ///////////////////

        void clear() noexcept;

        node_ptr insert(key_type key);

        void erase(const key_type& key);

        /////////////////
        //   LOOK UP   //
        /////////////////

        iterator find(const key_type& value);
        const_iterator find(const key_type& value) const;

        bool is_ordered(node_ptr subtree) const noexcept;

        bool is_balanced(node_ptr subtree) const noexcept;

        bool is_avl(node_ptr subtree = nullptr) const noexcept;

        std::pair<bool, int> check_balance_factors(node_ptr subtree) const noexcept;

    private:
        ///////////////////
        //   BALANCING   //
        ///////////////////

        node_ptr find_place(const key_type& value, bool last_nonzero = true) const;

        void update_balance_factors(node_ptr subtree);

        [[nodiscard]] node_ptr rotate_left(node_ptr subtree) noexcept;

        [[nodiscard]] node_ptr rotate_right(node_ptr subtree) noexcept;

        [[nodiscard]] node_ptr rotate_left_right(node_ptr subtree);

        [[nodiscard]] node_ptr rotate_right_left(node_ptr subtree);

        [[nodiscard]] node_ptr rebalance(node_ptr subtree);

        void clear_cache() const;

    private:
        node_ptr head = nullptr;
        std::size_t m_size = 0;
        key_compare key_cmp = { };

        mutable std::vector<bool> cmp_cache;
        mutable std::stack<node_ptr> path_cache;

        mutable node_ptr joint_to_branch = nullptr;
        mutable node_ptr branch_root = nullptr;
        mutable bool is_branch_right_child = false;
    };

} // namespace tree

// #include "detail/avl.tpp"
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

            if (last_nonzero &&
                current != nullptr &&
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
#pragma once

#include <iterator>
#include <cstddef>
#include <utility>
#include <vector>
#include <exception>
#include <optional>
#include <initializer_list>
#include <stack>
#include <queue>

// #include "detail/node.hpp"
// #include "iterator.hpp"

namespace tree
{
	template <typename Key, typename Compare = std::less<Key>>
	class splay
	{
	public:
		using key_type = Key;
		using key_compare = Compare;
		using node_type = tree::detail::Node<key_type>;
		using node_ptr = node_type *;
        using iterator = tree::NodeIterator<node_type>;
        using const_iterator = tree::NodeIterator<const node_type>;

	public:
		splay();

		splay(const std::initializer_list<key_type>& data);
		splay(std::initializer_list<key_type>&& data);

		splay(const splay<key_type, key_compare>& other);
		splay(splay<key_type, key_compare>&& other) noexcept;

		~splay();

		iterator begin();
		const_iterator begin() const;
		const_iterator cbegin() const;

		iterator end();
		const_iterator end() const;
		const_iterator cend() const;

		bool empty() const noexcept;
		std::size_t size() const noexcept;

		void clear() noexcept;

		node_ptr insert(key_type key);

		void erase(const key_type& key);

		iterator find(const key_type& value);
		const_iterator find(const key_type& value) const;

	private:

		node_ptr find_place(const key_type& value, bool last_nonzero = true) const;

		void clear_cache() const;

		void rotate_left(node_ptr v);

		void rotate_right(node_ptr v);

		void splay_operation(node_ptr v);

	private:
		node_ptr head = nullptr;
		std::size_t m_size = 0;
		key_compare key_cmp = { };

		mutable std::vector<bool> cmp_cache;
		mutable std::stack<node_ptr> path_cache;

	};
}

// #include "detail/splay.tpp"

#pragma once

#include <exception>
#include <iostream>

namespace tree
{

	template <typename Key, typename Compare>
	splay<Key, Compare>::splay() = default;

	template <typename Key, typename Compare>
	void splay<Key, Compare>::clear_cache() const
	{
		cmp_cache.clear();
		while (!path_cache.empty())
		{
			path_cache.pop();
		}
	}

	template <typename Key, typename Compare>
	splay<Key, Compare>::splay(const std::initializer_list<key_type>& data)
	{
		for (const auto& element : data)
		{
			this->insert(element);
		}
	}

	template <typename Key, typename Compare>
	splay<Key, Compare>::splay(std::initializer_list<key_type>&& data)
	{
		for (auto&& element : data)
		{
			this->insert(std::move(element));
		}
	}

	template <typename Key, typename Compare>
	splay<Key, Compare>::splay(const splay <key_type, key_compare>& other)
	{
		for (const auto& element : other)
		{
			this->insert(element);
		}
	}

	template <typename Key, typename Compare>
	splay<Key, Compare>::splay(splay <key_type, key_compare>&& other) noexcept
	{
		std::swap(this->head, other.head);
		std::swap(this->m_size, other.m_size);
	}

	template <typename Key, typename Compare>
	splay<Key, Compare>::~splay()
	{
		this->clear();
	}

        template <typename Key, typename Compare>
        typename splay<Key, Compare>::iterator splay<Key, Compare>::begin()
        {
                return iterator(head);
        }

        template <typename Key, typename Compare>
        typename splay<Key, Compare>::const_iterator splay<Key, Compare>::begin() const
        {
                return const_iterator(head);
        }

        template <typename Key, typename Compare>
        typename splay<Key, Compare>::const_iterator splay<Key, Compare>::cbegin() const
        {
                return const_iterator(head);
        }

        template <typename Key, typename Compare>
        typename splay<Key, Compare>::iterator splay<Key, Compare>::end()
        {
                return iterator(head, std::make_optional<node_ptr>(nullptr));
        }

        template <typename Key, typename Compare>
        typename splay<Key, Compare>::const_iterator splay<Key, Compare>::end() const
        {
                return const_iterator(head, std::make_optional<node_ptr>(nullptr));
        }

        template <typename Key, typename Compare>
        typename splay<Key, Compare>::const_iterator splay<Key, Compare>::cend() const
        {
                return const_iterator(std::make_optional<node_ptr>(nullptr));
        }

	template <typename Key, typename Compare>
	bool splay<Key, Compare>::empty() const noexcept
	{
		return size() == 0;
	}

	template <typename Key, typename Compare>
	std::size_t splay<Key, Compare>::size() const noexcept
	{
		return m_size;
	}



	template <typename Key, typename Compare>
	void splay<Key, Compare>::clear() noexcept
	{
		while (! this->empty())
		{
			erase(this->head->value);
		}
	}

	template <typename Key, typename Compare>
	typename splay<Key, Compare>::node_ptr splay<Key, Compare>::insert(key_type key)
	{
		node_ptr child = nullptr;
		if (head == nullptr)
		{
			head = new node_type(key);
			child = head;
		}
		else
		{
			node_ptr parent = head;
			node_ptr current = parent;
			while (current != nullptr) {
				if (current->value == key)
				{
					return current;
				}
				if (key_cmp(key, current->value))
				{
					parent = current;
					current = current->left;
				}
				else if (key_cmp(current->value, key))
				{
					parent = current;
					current = current->right;
				}
			}
			if (key_cmp(key, parent->value))
			{
				child = new node_type{ key };
				parent->left = child;
			}
			else if (key_cmp(parent->value, key))
			{
				child = new node_type{ key };
				parent->right = child;
			}
		}
		m_size++;
		splay_operation(child);
		return child;
	}

	template <typename Key, typename Compare>
	typename splay<Key, Compare>::iterator splay<Key, Compare>::find(const key_type& value)
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
		splay_operation(current);
		return iterator(head, current);
	}

        template <typename Key, typename Compare>
	typename splay<Key, Compare>::const_iterator splay<Key, Compare>::find(const key_type& value) const
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
		splay_operation(current);
		return const_iterator(head, current);
	}

	template <typename Key, typename Compare>
	void splay<Key, Compare>::erase(const key_type& key)
	{
		node_ptr current = head;
		node_ptr parent = head;
		bool is_left_child = false;
		while (current != nullptr)
		{
			if (key_cmp(key, current->value))
			{
				parent = current;
				current = current->left;
				is_left_child = true;
			}
			else if (key_cmp(current->value, key))
			{
				parent = current;
				current = current->right;
				is_left_child = false;
			}
			else
			{
				break;
			}
		}
		if (current == nullptr) {
			return;
		}
		else {
			node_ptr left_child = current->left;
			node_ptr right_child = current->right;
			if (is_left_child) {
				if (left_child != nullptr) {
					parent->left = left_child;
					node_ptr max = left_child;
					while (max->right != nullptr) {
						max = max->right;
					}
					max->right = right_child;
					delete current;
					splay_operation(max);
				}
				else {
					parent->left = right_child;
					delete current;
					if (right_child != nullptr) {
						splay_operation(right_child);
					}
					else {
						splay_operation(parent);
					}
				}
			}
			else {
				if (left_child != nullptr) {
					parent->right = left_child;
					node_ptr max = left_child;
					while (max->right != nullptr) {
						max = max->right;
					}
					max->right = right_child;
					if (current == head) {
						head = left_child;
					}
					delete current;
					splay_operation(max);
				}
				else {
					parent->right = right_child;
					if (current == head) {
                                            if (right_child != nullptr) {
						head = right_child;
					    }
					    else {
						head = nullptr;
						delete current;
						m_size--;
						return;
					    }
					}
					delete current;
					if (right_child != nullptr) {
						splay_operation(right_child);
					}
					else {
						splay_operation(parent);
					}
				}
			}
			m_size--;
		}
	}

	template <typename Key, typename Compare>
	typename splay<Key, Compare>::node_ptr
		splay<Key, Compare>::find_place(const key_type& value, bool last_nonzero) const
	{
		cmp_cache.clear();
		clear_cache();
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
		}

		return last_nonzero ? previous : current;
	}

	template <typename Key, typename Compare>
	void splay<Key, Compare>::rotate_left(node_ptr current)
	{
		find_place(current->value, 0);
		path_cache.pop();
		node_ptr parent = path_cache.top();
		path_cache.pop();
		if (!path_cache.empty()) {
			node_ptr grand_parent = path_cache.top();
			if (grand_parent->left == parent) {
				grand_parent->left = current;
			}
			else {
				grand_parent->right = current;
			}
			node_ptr tmp = current->left;
			current->left = parent;
			parent->right = tmp;
			clear_cache();
		}
		else {
			node_ptr tmp = current->left;
			current->left = parent;
			parent->right = tmp;
			head = current;
			clear_cache();
		}
	}

	template <typename Key, typename Compare>
	void splay<Key, Compare>::rotate_right(node_ptr current)
	{
		find_place(current->value, 0);
		path_cache.pop();
		node_ptr parent = path_cache.top();
		path_cache.pop();
		if (!path_cache.empty()) {
			node_ptr grand_parent = path_cache.top();
			if (grand_parent->left == parent) {
				grand_parent->left = current;
			}
			else {
				grand_parent->right = current;
			}
			node_ptr tmp = current->right;
			current->right = parent;
			parent->left = tmp;
			clear_cache();
		}
		else {
			node_ptr tmp = current->right;
			current->right = parent;
			parent->left = tmp;
			head = current;
			clear_cache();
		}
	}

	template <typename Key, typename Compare>
	void splay<Key, Compare>::splay_operation(node_ptr current)
	{
		if (current == nullptr) {
			return;
		}
		find_place(current->value, 0);
		std::stack<node_ptr> path = path_cache;
		if (path.size() == 1) {
			clear_cache();
			head = current;
			return;
		}
		while (!path.empty()) {
			if (path.size() == 1) {
				break;
			}
			path.pop();
			node_ptr parent = path.top();
			path.pop();
			if (parent->left == current) {
				if (path.empty()) {
					rotate_right(current);
				}
				else if (parent == path.top()->left) {
					rotate_right(parent);
					rotate_right(current);
				}
				else {
					rotate_right(current);
					rotate_left(current);
				}
			}
			else {
				if (path.empty()) {
					rotate_left(current);
				}
				else if (parent == path.top()->right) {
					rotate_left(parent);
					rotate_left(current);
				}
				else {
					rotate_left(current);
					rotate_right(current);
				}
			}
		}
		clear_cache();
		head = current;
	}

}
#pragma once

#include <iterator>
#include <cstddef>
#include <utility>
#include <vector>
#include <exception>
#include <optional>
#include <initializer_list>
#include <stack>
#include <queue>
#include <random>

// #include "detail/node.hpp"
// #include "iterator.hpp"

#define CARTESIAN_TREE_DEBUG_INSERT 0
#define CARTESIAN_TREE_DEBUG_ERASE 0

namespace tree
{
    template <typename Key, typename Compare = std::less<Key>>
    class cartesian
    {
    public:
        using key_type = Key;
        using key_compare = Compare;
        using node_type = tree::detail::NodeCartesian<key_type>;
        using node_ptr = node_type*;
        using iterator = tree::NodeIterator<node_type>;
        using const_iterator = tree::NodeIterator<const node_type>;
        using self_type = tree::cartesian<key_type, key_compare>;

    public:
        cartesian();

        cartesian(const std::initializer_list<key_type>& data);
        cartesian(std::initializer_list<key_type>&& data);

        cartesian(const cartesian<key_type, key_compare>& other);
        cartesian(cartesian<key_type, key_compare>&& other) noexcept;

        ~cartesian();

        cartesian<key_type, key_compare>& operator = (const cartesian<key_type, key_compare>& other);
        cartesian<key_type, key_compare>& operator = (cartesian<key_type, key_compare>&& other) noexcept;

        ///////////////////
        //   ITERATORS   //
        ///////////////////

        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;

        iterator end();
        const_iterator end() const;
        const_iterator cend() const;

        //////////////////
        //   CAPACITY   //
        //////////////////

        bool empty() const noexcept;
        std::size_t size() const noexcept;

        ///////////////////
        //   MODIFIERS   //
        ///////////////////

        void clear() noexcept;

        node_ptr insert(key_type key);

        void erase(const key_type& key);

        // possible memory leak if the returned value is discarded
        [[nodiscard]] std::pair<node_ptr, node_ptr> split(const key_type& key, node_ptr node);

        // possible memory leak if the returned value is discarded
        [[nodiscard]] node_ptr merge(node_ptr node_lhs, node_ptr node_rhs);

        /////////////////
        //   LOOK UP   //
        /////////////////

        iterator find(const key_type& value);
        const_iterator find(const key_type& value) const;

        bool is_ordered(node_ptr subtree) const noexcept;

        bool is_heap(node_ptr subtree) const noexcept;

        bool is_cartesian(node_ptr subtree = nullptr) const noexcept;

    private:
        auto get_random_priority() const;

    private:
        node_ptr head = nullptr;
        std::size_t m_size = 0;
        key_compare key_cmp = { };

        const unsigned seed = (std::random_device{})();
        mutable std::mt19937 gen = std::mt19937(seed);
        const int priority_min = std::numeric_limits<int>::min();
        const int priority_max = std::numeric_limits<int>::max();
        mutable std::uniform_int_distribution<> priority_dist
                = std::uniform_int_distribution(priority_min, priority_max);
    };

} // namespace tree

// #include "detail/cartesian.tpp"
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
        auto it = find(key);
        if (it != end())
        {
            return nullptr;
        }

        // step 1: create a new node with the given key and a random priority
        const auto priority = get_random_priority();
        auto child = new node_type(key, priority);

        // step 2: split the initial tree by the key and merge in the new node
        auto [lhs, rhs] = this->split(key, head);
        auto result = this->merge(lhs, child);
        result = this->merge(result, rhs);

        head = result;
        m_size++;
        return child;
    }

    template <typename Key, typename Compare>
    void cartesian<Key, Compare>::erase(const key_type& key)
    {
        auto it = find(key);
        if (it == end())
        {
            return;
        }

        // step 1: split by key
        auto [lhs, rhs] = this->split(key, head);
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
        else
        {
            lhs = to_delete->left;
        }

        delete to_delete;
        m_size--;

        // step 3: merge back
        this->head = merge(lhs, rhs);
    }

    template <typename Key, typename Compare>
    [[nodiscard]] std::pair<typename cartesian<Key, Compare>::node_ptr,
                            typename cartesian<Key, Compare>::node_ptr>
    cartesian<Key, Compare>::split(const key_type& key, node_ptr node)
    {
        if (node == nullptr)
        {
            return {nullptr, nullptr};
        }

        if (key_cmp(key, node->value))
        {
            node_ptr next = node->left;
            auto [lhs, rhs] = split(key, next);

            /*
            if (next != nullptr)
            {
                next->right = nullptr;
            }
            */

            node->left = rhs;

            return {lhs, node};
        }
        else
        {
            node_ptr next = node->right;
            auto [lhs, rhs] = split(key, next);

            /*
            if (next != nullptr)
            {
                next->left = nullptr;
            }
            */

            node->right = lhs;

            return {node, rhs};
        }
    }

    template <typename Key, typename Compare>
    [[nodiscard]]  typename cartesian<Key, Compare>::node_ptr
    cartesian<Key, Compare>::merge(node_ptr node_lhs, node_ptr node_rhs)
    {
        if (node_lhs == nullptr)
        {
            return node_rhs;
        }

        if (node_rhs == nullptr)
        {
            return node_lhs;
        }

        if (key_cmp(node_rhs->value, node_lhs->value))
        {
            std::swap(node_lhs, node_rhs);
        }

        node_ptr result = nullptr;

        if (node_lhs->priority < node_rhs->priority)
        {
            result = node_rhs;
            result->left = merge(node_lhs, result->left);
        }
        else
        {
            result = node_lhs;
            result->right = merge(result->right, node_rhs);
        }

        return result;
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
