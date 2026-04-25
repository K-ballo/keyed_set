// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("emplace() — inserts new element", "[keyed_set.modif]")
{
    M m;
    auto [it, ok] = m.emplace(test::Employee{1, "Alice"});

    REQUIRE(ok);
    CHECK(it->id == 1);
    CHECK(it->name == "Alice");
}

TEST_CASE("emplace() — duplicate key is rejected", "[keyed_set.modif]")
{
    M m{{1, "Alice"}};
    auto [it, ok] = m.emplace(test::Employee{1, "Duplicate"});

    CHECK(!ok);
    CHECK(it->name == "Alice");
    CHECK(m.size() == 1u);
}

TEST_CASE("emplace_hint() — inserts near hint", "[keyed_set.modif]")
{
    M m{{1, "Alice"}, {3, "Carol"}};
    auto it = m.emplace_hint(m.end(), test::Employee{2, "Bob"});

    CHECK(it->id == 2);
    CHECK(m.size() == 3u);
}
