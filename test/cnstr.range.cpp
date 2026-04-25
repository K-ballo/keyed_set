// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

#include <array>
#include <iterator>

TEST_CASE("keyed_set(i, j) — iterator-range constructor", "[keyed_set.cnstr]")
{
    std::array<test::Employee, 3> elems{{{1, "Alice"}, {2, "Bob"}, {3, "Carol"}}};

    eggs::keyed_set<test::Employee, &test::Employee::id> m(elems.begin(), elems.end());

    REQUIRE(m.size() == 3u);
    CHECK(m.contains(1));
    CHECK(m.contains(2));
    CHECK(m.contains(3));
}

TEST_CASE("keyed_set(i, j) — duplicate keys deduplicated", "[keyed_set.cnstr]")
{
    std::array<test::Employee, 4> elems{{{1, "Alice"}, {2, "Bob"}, {1, "Duplicate"}, {3, "Carol"}}};

    eggs::keyed_set<test::Employee, &test::Employee::id> m(elems.begin(), elems.end());

    CHECK(m.size() == 3u);
}
