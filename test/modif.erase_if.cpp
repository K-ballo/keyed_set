// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include "../fixture.hpp"
#include <catch.hpp>

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE(
    "erase_if — removes matching elements, returns count", "[keyed_set.modif]"
)
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}, {4, "D"}, {5, "E"}};

    auto n = eggs::erase_if(m, [](test::Employee const& e) {
        return e.id % 2 == 0;
    });

    CHECK(n == 2u);
    CHECK(m.size() == 3u);
    CHECK(!m.contains(2));
    CHECK(!m.contains(4));
    CHECK(m.contains(1));
    CHECK(m.contains(3));
    CHECK(m.contains(5));
}

TEST_CASE(
    "erase_if — no match returns 0 and leaves container unchanged",
    "[keyed_set.modif]"
)
{
    M m{{1, "A"}, {2, "B"}};
    auto n =
        eggs::erase_if(m, [](test::Employee const& e) { return e.id > 99; });
    CHECK(n == 0u);
    CHECK(m.size() == 2u);
}

TEST_CASE("erase_if — all match leaves container empty", "[keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}};
    auto n = eggs::erase_if(m, [](test::Employee const&) { return true; });
    CHECK(n == 3u);
    CHECK(m.empty());
}

TEST_CASE(
    "erase_if — predicate receives const reference to value_type",
    "[keyed_set.modif]"
)
{
    M m{{1, "Alice"}, {2, "Bob"}, {3, "Carol"}};
    eggs::erase_if(m, [](test::Employee const& e) { return e.name == "Bob"; });
    CHECK(m.size() == 2u);
    CHECK(!m.contains(2));
}
