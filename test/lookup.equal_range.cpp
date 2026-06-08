// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <iterator>

#include "fixture.hpp"
#include <catch.hpp>

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE(
    "equal_range(key_type const&) — present key, range of one",
    "[keyed_set.lookup]"
)
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}};

    auto [lo, hi] = m.equal_range(2);

    REQUIRE(lo != m.end());
    CHECK(lo->id == 2);
    CHECK(std::distance(lo, hi) == 1);
}

TEST_CASE(
    "equal_range(key_type const&) — absent key between elements, empty range",
    "[keyed_set.lookup]"
)
{
    M m{{1, "A"}, {3, "C"}};

    auto [lo, hi] = m.equal_range(2);

    CHECK(lo == hi);
    REQUIRE(lo != m.end());
    CHECK(lo->id == 3); // both point to first element > 2
}

TEST_CASE(
    "equal_range(key_type const&) — empty container returns [end, end)",
    "[keyed_set.lookup]"
)
{
    M m;

    auto [lo, hi] = m.equal_range(42);

    CHECK(lo == m.end());
    CHECK(hi == m.end());
    CHECK(lo == hi);
}

TEST_CASE(
    "equal_range(key_type const&) — key below all elements returns [begin, "
    "begin)",
    "[keyed_set.lookup]"
)
{
    M m{{5, "E"}, {10, "J"}, {15, "O"}};

    auto [lo, hi] = m.equal_range(1);

    CHECK(lo == hi);
    CHECK(lo == m.begin()); // both point to first element
}

TEST_CASE(
    "equal_range(key_type const&) — key above all elements returns [end, end)",
    "[keyed_set.lookup]"
)
{
    M m{{5, "E"}, {10, "J"}, {15, "O"}};

    auto [lo, hi] = m.equal_range(99);

    CHECK(lo == m.end());
    CHECK(hi == m.end());
}

TEST_CASE("equal_range(key_type const&) — first element", "[keyed_set.lookup]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}};

    auto [lo, hi] = m.equal_range(1);

    REQUIRE(lo != m.end());
    CHECK(lo == m.begin());
    CHECK(lo->id == 1);
    CHECK(std::distance(lo, hi) == 1);
}

TEST_CASE("equal_range(key_type const&) — last element", "[keyed_set.lookup]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}};

    auto [lo, hi] = m.equal_range(3);

    REQUIRE(lo != m.end());
    CHECK(lo->id == 3);
    CHECK(hi == m.end());
}

TEST_CASE("equal_range(K const&) — transparent overload", "[keyed_set.lookup]")
{
    M m{{5, "E"}, {10, "J"}};

    auto [lo, hi] = m.equal_range(static_cast<long>(5));

    REQUIRE(lo != m.end());
    CHECK(lo->id == 5);
    CHECK(std::distance(lo, hi) == 1);
}

TEST_CASE(
    "equal_range — consistent with lower_bound and upper_bound",
    "[keyed_set.lookup]"
)
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}, {4, "D"}, {5, "E"}};

    for (int k : {0, 1, 2, 3, 4, 5, 6}) {
        auto [lo, hi] = m.equal_range(k);
        CHECK(lo == m.lower_bound(k));
        CHECK(hi == m.upper_bound(k));
    }
}
