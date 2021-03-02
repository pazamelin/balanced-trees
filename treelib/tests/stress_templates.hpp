#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include <catch2/catch_all.hpp>

#include <random>
#include <iostream>
#include <memory>
#include <set>

#include "seed.hpp"
#include "operations.hpp"
#include "avl.hpp"

namespace tree::testing
{
    template <typename TreeLHS, typename TreeRHS,
              typename Comparator>
    void stress_insert(Comparator cmp_trees,
                       unsigned int seed,
                       int key_lhs = 0, int key_rhs = 100,
                       std::size_t number_of_iterations = 100,
                       std::size_t insertions_per_iteration = 100
    )
    {
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> key_dist(key_lhs, key_rhs);
        auto get_random_key = [&]()
        { return key_dist(gen); };

        TreeLHS tree_lhs;
        TreeRHS tree_rhs;

        for (std::size_t iter = 0; iter < number_of_iterations; iter++)
        {
            for (std::size_t ins = 0; ins < insertions_per_iteration; ins++)
            {
                auto key = get_random_key();

                tree_lhs.insert(key);
                tree_rhs.insert(key);

                cmp_trees(tree_lhs, tree_rhs);
            }

            tree_lhs.clear();
            tree_rhs.clear();
        }
    }

    template <typename TreeLHS,
              typename TreeRHS,
              typename Comparator>
    void stress_erase(Comparator cmp_trees,
                      unsigned int seed,
                      int key_lhs = 0, int key_rhs = 100,
                      std::size_t starting_tree_size = 100,
                      std::size_t number_of_iterations = 100,
                      std::size_t erases_per_iteration = 100
    )
    {
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> key_dist(key_lhs, key_rhs);
        auto get_random_key = [&]() { return key_dist(gen); };

        const std::vector<int> keys = [&]()
        {
            std::vector<int> keys;
            keys.reserve(starting_tree_size);

            for (std::size_t i = 0; i < starting_tree_size; i++)
            {
                keys.push_back(get_random_key());
            }

            return keys;
        }( );

        TreeLHS tree_lhs;
        TreeRHS tree_rhs;

        for (auto key : keys)
        {
            tree_lhs.insert(key);
            tree_rhs.insert(key);
        }

        cmp_trees(tree_lhs, tree_rhs);

        for (std::size_t iter = 0; iter < number_of_iterations; iter++)
        {
            for (std::size_t erase = 0; erase < erases_per_iteration; erase++)
            {
                auto key = get_random_key();

                tree_lhs.erase(key);
                tree_rhs.erase(key);

                cmp_trees(tree_lhs, tree_rhs);
            }

            tree_lhs.clear();
            tree_rhs.clear();
        }
    }

    template <typename TreeLHS,
              typename TreeRHS,
              typename Comparator>
    void stress_find(Comparator cmp_trees,
                     unsigned int seed,
                     int key_lhs = 0, int key_rhs = 100,
                     std::size_t starting_tree_size = 100,
                     std::size_t number_of_iterations = 100,
                     std::size_t finds_per_iteration = 100
    )
    {
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> key_dist(key_lhs, key_rhs);
        auto get_random_key = [&]() { return key_dist(gen); };

        const std::vector<int> keys = [&]()
        {
            std::vector<int> keys;
            keys.reserve(starting_tree_size);

            for (std::size_t i = 0; i < starting_tree_size; i++)
            {
                keys.push_back(get_random_key());
            }

            return keys;
        }( );

        TreeLHS tree_lhs;
        TreeRHS tree_rhs;

        for (auto key : keys)
        {
            tree_lhs.insert(key);
            tree_rhs.insert(key);
        }

        cmp_trees(tree_lhs, tree_rhs);

        for (std::size_t iter = 0; iter < number_of_iterations; iter++)
        {
            for (std::size_t find = 0; find < finds_per_iteration; find++)
            {
                auto key = get_random_key();

                auto lhs_it = tree_lhs.find(key);
                auto rhs_it = tree_rhs.find(key);

                bool found_in_lhs = (lhs_it != tree_lhs.end());
                if (found_in_lhs)
                {
                    REQUIRE(*lhs_it == key);
                }

                bool found_in_rhs = (rhs_it != tree_rhs.end());
                if (found_in_rhs)
                {
                    REQUIRE(*rhs_it == key);
                }

                REQUIRE(found_in_lhs == found_in_rhs);

                cmp_trees(tree_lhs, tree_rhs);
            }

            tree_lhs.clear();
            tree_rhs.clear();
        }
    }

    template <typename TreeLHS, typename TreeRHS,
            typename Comparator>
    void stress_mixed(Comparator cmp_trees,
                      unsigned int seed,
                      int key_lhs = -1000, int key_rhs = 1000,
                      std::size_t number_of_iterations = 100,
                      std::size_t operations_per_iteration = 1000
    )
    {
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> key_dist(key_lhs, key_rhs);
        auto get_random_key = [&]() { return key_dist(gen); };

        std::uniform_int_distribution<> operation_dist(0, 99);
        auto get_random_operation = [&]() { return get_operation(operation_dist(gen)); };

        TreeLHS tree_lhs;
        TreeRHS tree_rhs;

        for (std::size_t iter = 0; iter < number_of_iterations; iter++)
        {
            for (std::size_t ins = 0; ins < operations_per_iteration; ins++)
            {
                const auto key = get_random_key();
                const auto operation = get_random_operation();

                switch (operation)
                {
                    case eOperation::insert:
                    {
                        tree_lhs.insert(key);
                        tree_rhs.insert(key);
                        break;
                    }
                    case eOperation::erase:
                    {
                        tree_lhs.erase(key);
                        tree_rhs.erase(key);
                        break;
                    }
                    case eOperation::find:
                    {
                        auto lhs_it = tree_lhs.find(key);
                        auto rhs_it = tree_rhs.find(key);

                        bool found_in_lhs = (lhs_it != tree_lhs.end());
                        if (found_in_lhs)
                        {
                            REQUIRE(*lhs_it == key);
                        }

                        bool found_in_rhs = (rhs_it != tree_rhs.end());
                        if (found_in_rhs)
                        {
                            REQUIRE(*rhs_it == key);
                        }

                        REQUIRE(found_in_lhs == found_in_rhs);

                        break;
                    }
                }

                cmp_trees(tree_lhs, tree_rhs);
            }

            tree_lhs.clear();
            tree_rhs.clear();
        }
    }

} // namespace tree::testing
