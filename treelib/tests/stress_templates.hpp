#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include <catch2/catch_all.hpp>

#include <random>
#include <set>
#include <iostream>
#include <memory>

#include "avl.hpp"

#define SEED RAND

namespace tree::testing
{
    auto get_seed()
    {
#if SEED == RAND
        std::random_device rd;
        const auto seed = rd();
#else
        const auto seed = SEED;
#endif
        return seed;
    }

    template <typename TreeLHS, typename TreeRHS,
              typename Comparator>
    void stress_insert(Comparator cmp_trees,
                       unsigned int seed,
                       int key_lhs = 0, int key_rhs = 100,
                       std::size_t number_of_iterations = 100,
                       std::size_t insertions_per_iterations = 100
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
            for (std::size_t ins = 0; ins < insertions_per_iterations; ins++)
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
                      std::size_t erases_per_iterations = 100
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
            for (std::size_t erase = 0; erase < erases_per_iterations; erase++)
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

    template <typename T>
    void compare_traverse(tree::avl<T>& avl_tree, const std::set<T>& rb_tree)
    {
        REQUIRE(avl_tree.is_avl());
        REQUIRE(avl_tree.size() == rb_tree.size());

        auto rb_it = rb_tree.cbegin();
        for (auto element : avl_tree)
        {
            REQUIRE(element == *rb_it++);
        }
    }

} // namespace tree::testing
