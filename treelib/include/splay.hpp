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

#include "detail/splay.tpp"
