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
}