// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

TEST_CASE("keyed_set(il) — initializer-list constructor", "[keyed_set.cnstr]")
{
    eggs::keyed_set<test::Employee, &test::Employee::id> m{
        {1, "Alice"},
        {2, "Bob"},
        {3, "Carol"},
    };

    REQUIRE(m.size() == 3u);
    CHECK(m.find(1)->name == "Alice");
    CHECK(m.find(2)->name == "Bob");
    CHECK(m.find(3)->name == "Carol");
}
