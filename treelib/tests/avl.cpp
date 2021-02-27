#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>

#include <set>
#include "avl.hpp"

TEST_CASE("one left rotation", "[avl_tree]")
{
    tree::avl<int> avl_tree{3, 2, 1};
    REQUIRE(avl_tree.is_avl());
    REQUIRE(avl_tree.size() == 3);
}

TEST_CASE("one right rotation", "[avl_tree]")
{
    tree::avl<int> avl_tree{1, 2, 3};
    REQUIRE(avl_tree.is_avl());
    REQUIRE(avl_tree.size() == 3);
}

TEST_CASE("one double right-left rotation", "[avl_tree]")
{
    tree::avl<int> avl_tree{5, 0, 20, 15, 10, 25};

    REQUIRE(avl_tree.is_avl());
    REQUIRE(avl_tree.size() == 6);
}

TEST_CASE("one double left-right rotation", "[avl_tree]")
{
    tree::avl<int> avl_tree{25, 10, 30, 5, 15, 20};

    REQUIRE(avl_tree.is_avl());
    REQUIRE(avl_tree.size() == 6);
}

TEST_CASE("increasing order", "[avl_tree]")
{
    tree::avl<int> avl_tree{1, 2, 3, 4, 5, 6};

    REQUIRE(avl_tree.is_avl());
    REQUIRE(avl_tree.size() == 6);
}

TEST_CASE("decreasing order", "[avl_tree]")
{
    tree::avl<int> avl_tree{6, 5, 4, 3, 2, 1};

    REQUIRE(avl_tree.is_avl());
    REQUIRE(avl_tree.size() == 6);
}

TEST_CASE("traversing", "[avl_tree]")
{
    tree::avl<int> avl_tree = {1, 2, 3, 4, 5, 6};

    REQUIRE(avl_tree.is_avl());
    REQUIRE(avl_tree.size() == 6);

    const std::vector<int> expected_order = {1, 2, 3, 4, 5, 6};
    auto expected_it = expected_order.cbegin();

    for (const auto& element : avl_tree)
    {
        REQUIRE(element == *expected_it++);
    }
}

TEST_CASE("traversing back", "[avl_tree]")
{
    tree::avl<int> avl_tree = {1, 2, 3, 4, 5, 6};

    REQUIRE(avl_tree.is_avl());
    REQUIRE(avl_tree.size() == 6);

    const std::vector<int> expected_order = {6, 5, 4, 3, 2, 1};
    auto expected_it = expected_order.cbegin();

    auto it = std::prev(avl_tree.end());
    while (it != avl_tree.begin())
    {
        REQUIRE(*it-- == *expected_it++);
    }
    REQUIRE(*it == *expected_it);
}

TEST_CASE("find", "[avl_tree]")
{
    const std::vector<int> data = {1, 2, 3, 4, 5, 6};
    tree::avl<int> avl_tree = {1, 2, 3, 4, 5, 6};

    for (auto element : data)
    {
        auto element_it = avl_tree.find(element);
        REQUIRE(*element_it == element);
    }
}

template<typename T>
void check_traverse(tree::avl<T>& avl_tree, const std::set<T>& rb_tree)
{
    REQUIRE(avl_tree.is_avl());
    REQUIRE(avl_tree.size() == rb_tree.size());

    auto rb_it = rb_tree.cbegin();
    for (auto element : avl_tree)
    {
        REQUIRE(element == *rb_it++);
    }
}

TEST_CASE("erase", "[avl_tree]")
{
    const std::initializer_list<int> data = {5, 2, 15, 1, 3, 10, 20, 4, 6, 12, 25, 7};

    tree::avl<int> avl_tree = data;
    std::set<int> rb_tree = data;
    check_traverse(avl_tree, rb_tree);

    std::vector<int> erase_order = {5, 6, 7, 10, 12, 15, 20, 25, 2, 3, 4, 1};

    for (auto key : erase_order)
    {
        avl_tree.erase(key);
        rb_tree.erase(key);
        check_traverse(avl_tree, rb_tree);
    }
}
