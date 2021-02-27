#pragma once

#include <stack>
#include <optional>

#include "detail/node.hpp"

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

#include "detail/iterator.tpp"