// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("find(key_type const&) — returns iterator to element", "[keyed_set.lookup]")
{
    M m{{1, "Alice"}, {2, "Bob"}};

    auto it = m.find(1);
    REQUIRE(it != m.end());
    CHECK(it->name == "Alice");
}

TEST_CASE("find(key_type const&) — missing key returns end()", "[keyed_set.lookup]")
{
    M m{{1, "Alice"}};

    CHECK(m.find(99) == m.end());
}

TEST_CASE("find(key_type const&) const — const overload", "[keyed_set.lookup]")
{
    M const m{{1, "Alice"}};

    auto it = m.find(1);
    REQUIRE(it != m.end());
    CHECK(it->name == "Alice");
}

TEST_CASE("find(K const&) — transparent lookup, no key_type construction",
          "[keyed_set.lookup]")
{
    // Pass a long; the comparator's is_transparent allows this without
    // constructing an int.
    M m{{10, "X"}, {20, "Y"}};

    auto it = m.find(static_cast<long>(10));
    REQUIRE(it != m.end());
    CHECK(it->id == 10);
}

TEST_CASE("find — iterating in key order", "[keyed_set.lookup]")
{
    M m{{3, "C"}, {1, "A"}, {2, "B"}};

    int prev = -1;
    for (auto const& e : m)
    {
        CHECK(e.id > prev);
        prev = e.id;
    }
}
