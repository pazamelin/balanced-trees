#include "stress_templates.hpp"
#include "comparators.hpp"

TEST_CASE("stress test, insert", "[avl-rb]")
{
    using TreeLHS = tree::avl<int>;
    using TreeRHS = std::set<int>;
    auto cmp = &tree::testing::compare_traverse<int>;
    auto seed = tree::testing::get_seed();

    tree::testing::stress_insert<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress test, erase", "[avl-rb]")
{
    using TreeLHS = tree::avl<int>;
    using TreeRHS = std::set<int>;
    auto cmp = &tree::testing::compare_traverse<int>;
    auto seed = tree::testing::get_seed();

    tree::testing::stress_erase<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress test, find", "[avl-rb]")
{
    using TreeLHS = tree::avl<int>;
    using TreeRHS = std::set<int>;
    auto cmp = &tree::testing::compare_traverse<int>;
    auto seed = tree::testing::get_seed();

    tree::testing::stress_find<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress mixed", "[avl-rb]")
{
    using TreeLHS = tree::avl<int>;
    using TreeRHS = std::set<int>;
    auto cmp = &tree::testing::compare_traverse<int>;
    auto seed = tree::testing::get_seed();

    tree::testing::stress_mixed<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress test, insert", "[splay-rb]")
{
	using TreeLHS = tree::splay<int>;
	using TreeRHS = std::set<int>;
	auto cmp = &tree::testing::compare_traverse_splay<int>;
	auto seed = tree::testing::get_seed();

	tree::testing::stress_insert<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress test, erase", "[splay-rb]")
{
	using TreeLHS = tree::splay<int>;
	using TreeRHS = std::set<int>;
	auto cmp = &tree::testing::compare_traverse_splay<int>;
	auto seed = tree::testing::get_seed();

	tree::testing::stress_erase<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress test, find", "[splay-rb]")
{
	using TreeLHS = tree::splay<int>;
	using TreeRHS = std::set<int>;
	auto cmp = &tree::testing::compare_traverse_splay<int>;
	auto seed = tree::testing::get_seed();

	tree::testing::stress_find<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress mixed", "[splay-rb]")
{
	using TreeLHS = tree::splay<int>;
	using TreeRHS = std::set<int>;
	auto cmp = &tree::testing::compare_traverse_splay<int>;
	auto seed = tree::testing::get_seed();

	tree::testing::stress_mixed<TreeLHS, TreeRHS>(cmp, seed);
}