#include "stress_templates.hpp"
#include "comparators.hpp"
#include "seed.hpp"

/////////////////////////
//   AVL - RED-BLACK   //
/////////////////////////

TEST_CASE("stress test, insert, avl", "[avl-rb]")
{
    using TreeLHS = tree::avl<int>;
    using TreeRHS = std::set<int>;
    auto cmp = &tree::testing::compare_traverse<int>;
    auto seed = tree::testing::get_seed();

    tree::testing::stress_insert<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress test, erase, avl", "[avl-rb]")
{
    using TreeLHS = tree::avl<int>;
    using TreeRHS = std::set<int>;
    auto cmp = &tree::testing::compare_traverse<int>;
    auto seed = tree::testing::get_seed();

    tree::testing::stress_erase<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress test, find, avl", "[avl-rb]")
{
    using TreeLHS = tree::avl<int>;
    using TreeRHS = std::set<int>;
    auto cmp = &tree::testing::compare_traverse<int>;
    auto seed = tree::testing::get_seed();

    tree::testing::stress_find<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress mixed, avl", "[avl-rb]")
{
    using TreeLHS = tree::avl<int>;
    using TreeRHS = std::set<int>;
    auto cmp = &tree::testing::compare_traverse<int>;
    auto seed = tree::testing::get_seed();

    tree::testing::stress_mixed<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress test, insert, splay", "[splay-rb]")
{
	using TreeLHS = tree::splay<int>;
	using TreeRHS = std::set<int>;
	auto cmp = &tree::testing::compare_traverse_splay<int>;
	auto seed = tree::testing::get_seed();

	tree::testing::stress_insert<TreeLHS, TreeRHS>(cmp, seed);
}

///////////////////////////
//   SPLAY - RED-BLACK   //
///////////////////////////

TEST_CASE("stress test, erase, splay", "[splay-rb]")
{
	using TreeLHS = tree::splay<int>;
	using TreeRHS = std::set<int>;
	auto cmp = &tree::testing::compare_traverse_splay<int>;
	auto seed = tree::testing::get_seed();

	tree::testing::stress_erase<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress test, find, splay", "[splay-rb]")
{
	using TreeLHS = tree::splay<int>;
	using TreeRHS = std::set<int>;
	auto cmp = &tree::testing::compare_traverse_splay<int>;
	auto seed = tree::testing::get_seed();

	tree::testing::stress_find<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress mixed, splay", "[splay-rb]")
{
	using TreeLHS = tree::splay<int>;
	using TreeRHS = std::set<int>;
	auto cmp = &tree::testing::compare_traverse_splay<int>;
	auto seed = tree::testing::get_seed();

	tree::testing::stress_mixed<TreeLHS, TreeRHS>(cmp, seed);
}

///////////////////////////////
//   CARTESIAN - RED-BLACK   //
///////////////////////////////

TEST_CASE("stress test, insert, cartesian", "[cartesian-rb]")
{
    using TreeLHS = tree::cartesian<int>;
    using TreeRHS = std::set<int>;
    auto cmp = &tree::testing::compare_traverse_cartesian<int>;
    auto seed = tree::testing::get_seed();

    tree::testing::stress_insert<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress test, erase, cartesian", "[cartesian-rb]")
{
    using TreeLHS = tree::cartesian<int>;
    using TreeRHS = std::set<int>;
    auto cmp = &tree::testing::compare_traverse_cartesian<int>;
    auto seed = tree::testing::get_seed();

    tree::testing::stress_erase<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress test, find, cartesian", "[cartesian-rb]")
{
    using TreeLHS = tree::cartesian<int>;
    using TreeRHS = std::set<int>;
    auto cmp = &tree::testing::compare_traverse_cartesian<int>;
    auto seed = tree::testing::get_seed();

    tree::testing::stress_find<TreeLHS, TreeRHS>(cmp, seed);
}

TEST_CASE("stress mixed, cartesian", "[cartesian-rb]")
{
    using TreeLHS = tree::cartesian<int>;
    using TreeRHS = std::set<int>;
    auto cmp = &tree::testing::compare_traverse_cartesian<int>;
    auto seed = tree::testing::get_seed();

    tree::testing::stress_mixed<TreeLHS, TreeRHS>(cmp, seed);
}