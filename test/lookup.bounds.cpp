// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("lower_bound(key_type const&) — first element not less than k",
          "[keyed_set.lookup]")
{
    M m{{1, "A"}, {3, "C"}, {5, "E"}};

    auto it = m.lower_bound(3);
    REQUIRE(it != m.end());
    CHECK(it->id == 3);
}

TEST_CASE("lower_bound(key_type const&) — between elements", "[keyed_set.lookup]")
{
    M m{{1, "A"}, {3, "C"}, {5, "E"}};

    auto it = m.lower_bound(2);
    REQUIRE(it != m.end());
    CHECK(it->id == 3);
}

TEST_CASE("lower_bound(key_type const&) — past last returns end()",
          "[keyed_set.lookup]")
{
    M m{{1, "A"}, {3, "C"}};
    CHECK(m.lower_bound(99) == m.end());
}

TEST_CASE("upper_bound(key_type const&) — first element greater than k",
          "[keyed_set.lookup]")
{
    M m{{1, "A"}, {3, "C"}, {5, "E"}};

    auto it = m.upper_bound(3);
    REQUIRE(it != m.end());
    CHECK(it->id == 5);
}

TEST_CASE("upper_bound(key_type const&) — past last returns end()",
          "[keyed_set.lookup]")
{
    M m{{1, "A"}, {3, "C"}};
    CHECK(m.upper_bound(99) == m.end());
}

TEST_CASE("lower_bound / upper_bound — transparent overloads", "[keyed_set.lookup]")
{
    M m{{10, "X"}, {20, "Y"}, {30, "Z"}};

    auto lo = m.lower_bound(static_cast<long>(20));
    auto hi = m.upper_bound(static_cast<long>(20));

    REQUIRE(lo != m.end());
    CHECK(lo->id == 20);
    REQUIRE(hi != m.end());
    CHECK(hi->id == 30);
}
