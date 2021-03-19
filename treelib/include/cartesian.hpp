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

#include "detail/node.hpp"
#include "iterator.hpp"

#define CARTESIAN_TREE_DEBUG_ROTATIONS 0
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

        // guaranteed memory leak if the returned value is discarded
        [[nodiscard]] std::pair<node_ptr, node_ptr> split(const key_type& key, node_ptr node);

        // guaranteed memory leak if the returned value is discarded
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

        //const unsigned seed = (std::random_device{})();
        const unsigned seed = 11;
        mutable std::mt19937 gen = std::mt19937(seed);
        const int priority_min = std::numeric_limits<int>::min();
        const int priority_max = std::numeric_limits<int>::max();
        mutable std::uniform_int_distribution<> priority_dist
                = std::uniform_int_distribution(priority_min, priority_max);
    };

} // namespace tree

#include "detail/cartesian.tpp"