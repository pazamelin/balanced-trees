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

#include "detail/node.hpp"
#include "iterator.hpp"

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

        std::pair<bool, int> check_balance_factors(node_ptr subtree) const;

    private:
        ///////////////////
        //   BALANCING   //
        ///////////////////

        node_ptr find_place(const key_type& value, bool last_nonzero = true) const;

        void update_balance_factors(node_ptr subtree);

        [[nodiscard]] node_ptr rotate_left(node_ptr subtree) noexcept;

        [[nodiscard]] node_ptr rotate_right(node_ptr subtree) noexcept;

        [[nodiscard]] node_ptr rotate_left_right(node_ptr subtree) noexcept;

        [[nodiscard]] node_ptr rotate_right_left(node_ptr subtree) noexcept;

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

#include "detail/avl.tpp"