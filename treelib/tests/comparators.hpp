#include <set>

#include "avl.hpp"
#include "splay.hpp"
#include "cartesian.hpp"

namespace tree::testing
{
    template <typename T>
    void compare_traverse(tree::avl<T>& avl_tree, const std::set<T>& rb_tree)
    {
        REQUIRE(avl_tree.size() == rb_tree.size());
        REQUIRE(avl_tree.is_avl());

        auto rb_it = rb_tree.cbegin();
        for (auto avl_element : avl_tree)
        {
            REQUIRE(avl_element == *rb_it++);
        }
    }

	template <typename T>
	void compare_traverse_splay(tree::splay<T>& splay_tree, const std::set<T>& rb_tree)
	{
		REQUIRE(splay_tree.size() == rb_tree.size());

		auto rb_it = rb_tree.cbegin();
		for (auto splay_element : splay_tree)
		{
			REQUIRE(splay_element == *rb_it++);
		}
	}

    template <typename T>
    void compare_traverse_cartesian(tree::cartesian<T>& cartesian_tree, const std::set<T>& rb_tree)
    {
        REQUIRE(cartesian_tree.size() == rb_tree.size());
        REQUIRE(cartesian_tree.is_cartesian());

        auto rb_it = rb_tree.cbegin();
        for (auto cartesian_tree_element : cartesian_tree)
        {
            REQUIRE(cartesian_tree_element == *rb_it++);
        }
    }

} // namespace tree::testing