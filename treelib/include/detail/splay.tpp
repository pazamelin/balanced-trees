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
		while (not this->empty())
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
	typename splay<Key, Compare>::node_ptr splay<Key, Compare>::find(const key_type& value)
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
		return current;
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
						head = left_child;
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