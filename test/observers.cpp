// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <functional>
#include <type_traits>

#include "fixture.hpp"
#include <catch.hpp>

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("key_comp() — returns the Compare object", "[keyed_set.obs]")
{
    M m;
    auto cmp = m.key_comp();

    // key_compare is the user-supplied Compare, not the projecting wrapper
    static_assert(std::is_same_v<decltype(cmp), M::key_compare>);
    static_assert(std::is_same_v<M::key_compare, std::less<int>>);

    // key_comp() compares key_type values directly
    CHECK(cmp(1, 2));
    CHECK(!cmp(2, 1));
    CHECK(!cmp(1, 1));
}

TEST_CASE(
    "value_comp() — returns the projecting comparator over value_type",
    "[keyed_set.obs]"
)
{
    M m;

    // value_compare is the projecting adaptor, distinct from key_compare
    static_assert(!std::is_same_v<M::key_compare, M::value_compare>);

    auto vc = m.value_comp();
    static_assert(std::is_same_v<decltype(vc), M::value_compare>);

    test::Employee a{1, "A"}, b{2, "B"};
    CHECK(vc(a, b));
    CHECK(!vc(b, a));
    CHECK(!vc(a, a));
}

TEST_CASE(
    "key_compare::is_transparent — present only when Compare is transparent",
    "[keyed_set.obs]"
)
{
    // std::less<int> is not transparent
    static_assert(!test::transparent<M::key_compare>);

    // std::less<> is transparent
    using Trans =
        eggs::keyed_set<test::Employee, &test::Employee::id, std::less<>>;
    static_assert(test::transparent<Trans::key_compare>);
}

TEST_CASE("value_compare::is_transparent — always present", "[keyed_set.obs]")
{
    // value_compare (the projecting comparator) always defines is_transparent
    // so that std::set activates its heterogeneous tree operations
    static_assert(test::transparent<M::value_compare>);
}
