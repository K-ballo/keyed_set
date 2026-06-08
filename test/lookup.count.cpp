// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include "fixture.hpp"
#include <catch.hpp>

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE(
    "count(key_type const&) — present key returns 1", "[keyed_set.lookup]"
)
{
    M m{{1, "Alice"}, {2, "Bob"}};
    CHECK(m.count(1) == 1u);
}

TEST_CASE("count(key_type const&) — absent key returns 0", "[keyed_set.lookup]")
{
    M m{{1, "Alice"}};
    CHECK(m.count(99) == 0u);
}

TEST_CASE("count(K const&) — transparent overload", "[keyed_set.lookup]")
{
    M m{{5, "E"}};
    CHECK(m.count(static_cast<long>(5)) == 1u);
    CHECK(m.count(static_cast<long>(6)) == 0u);
}
