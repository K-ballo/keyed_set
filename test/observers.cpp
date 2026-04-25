// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

#include <type_traits>

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("key_comp() — returns the comparison object", "[keyed_set.obs]")
{
    M m;
    auto cmp = m.key_comp();

    static_assert(std::is_same_v<decltype(cmp), M::key_compare>);

    test::Employee a{1, "A"}, b{2, "B"};
    CHECK(cmp(a, b));
    CHECK(!cmp(b, a));
    CHECK(!cmp(a, a));
}

TEST_CASE("value_comp() — same type as key_comp() for set-like container",
          "[keyed_set.obs]")
{
    M m;
    static_assert(std::is_same_v<M::key_compare, M::value_compare>);

    auto vc = m.value_comp();
    test::Employee a{1, "A"}, b{2, "B"};
    CHECK(vc(a, b));
}

TEST_CASE("key_comp() — is_transparent defined", "[keyed_set.obs]")
{
    static_assert(requires { typename M::key_compare::is_transparent; });
}
