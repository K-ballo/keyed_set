// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

TEST_CASE("clear() — postcondition: empty()", "[keyed_set.modif]")
{
    eggs::keyed_set<test::Employee, &test::Employee::id> m{
        {1, "Alice"}, {2, "Bob"}, {3, "Carol"}};

    m.clear();

    CHECK(m.empty());
    CHECK(m.size() == 0u);
    CHECK(m.begin() == m.end());
}
