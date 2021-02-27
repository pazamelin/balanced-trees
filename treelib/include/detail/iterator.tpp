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
            if (not node_stack.empty())
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
        return not (*this == other);
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