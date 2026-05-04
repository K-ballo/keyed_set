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

TEST_CASE("emplace() — returned iterator points to existing element on failure",
          "[keyed_set.modif]")
{
    M m{{1, "Alice"}, {2, "Bob"}};

    auto [it, ok] = m.emplace(test::Employee{2, "NotBob"});

    CHECK(!ok);
    CHECK(it->id == 2);
    CHECK(it->name == "Bob");
}

TEST_CASE("emplace_hint() — correct hint at end()", "[keyed_set.modif]")
{
    M m{{1, "Alice"}, {3, "Carol"}};
    auto it = m.emplace_hint(m.end(), test::Employee{2, "Bob"});

    CHECK(it->id == 2);
    CHECK(m.size() == 3u);
}

TEST_CASE("emplace_hint() — correct hint at begin()", "[keyed_set.modif]")
{
    M m{{2, "Bob"}, {3, "Carol"}};
    auto it = m.emplace_hint(m.begin(), test::Employee{1, "Alice"});

    CHECK(it->id == 1);
    CHECK(it == m.begin());
    CHECK(m.size() == 3u);
}

TEST_CASE("emplace_hint() — wrong hint still produces correct result",
          "[keyed_set.modif]")
{
    M m{{1, "Alice"}, {2, "Bob"}, {3, "Carol"}};

    // Deliberately wrong hint: element 5 belongs at end, but we hint begin
    auto it = m.emplace_hint(m.begin(), test::Employee{5, "Eve"});

    CHECK(it->id == 5);
    CHECK(m.size() == 4u);
    // Container must still be ordered correctly
    int prev = -1;
    for (auto const& e : m)
    {
        CHECK(e.id > prev);
        prev = e.id;
    }
}

TEST_CASE("emplace_hint() — duplicate key with any hint is rejected",
          "[keyed_set.modif]")
{
    M m{{1, "Alice"}, {2, "Bob"}};

    // Wrong hint (end), duplicate key
    auto it = m.emplace_hint(m.end(), test::Employee{1, "Duplicate"});

    CHECK(it->name == "Alice");  // existing element
    CHECK(m.size() == 2u);
}
