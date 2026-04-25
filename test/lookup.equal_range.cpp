// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

#include <iterator>

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("equal_range(key_type const&) — present key, range of one",
          "[keyed_set.lookup]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}};

    auto [lo, hi] = m.equal_range(2);

    REQUIRE(lo != m.end());
    CHECK(lo->id == 2);
    CHECK(std::distance(lo, hi) == 1);
}

TEST_CASE("equal_range(key_type const&) — absent key, empty range",
          "[keyed_set.lookup]")
{
    M m{{1, "A"}, {3, "C"}};

    auto [lo, hi] = m.equal_range(2);

    CHECK(lo == hi);
    // Both point to the first element greater than 2
    REQUIRE(lo != m.end());
    CHECK(lo->id == 3);
}

TEST_CASE("equal_range(K const&) — transparent overload", "[keyed_set.lookup]")
{
    M m{{5, "E"}, {10, "J"}};

    auto [lo, hi] = m.equal_range(static_cast<long>(5));

    REQUIRE(lo != m.end());
    CHECK(lo->id == 5);
    CHECK(std::distance(lo, hi) == 1);
}
