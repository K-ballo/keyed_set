// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("keyed_set(const keyed_set&) — copy constructor", "[keyed_set.cnstr]")
{
    M src{{1, "Alice"}, {2, "Bob"}};
    M copy(src);

    CHECK(copy == src);
    CHECK(copy.size() == 2u);
}

TEST_CASE("keyed_set(keyed_set&&) — move constructor", "[keyed_set.cnstr]")
{
    M src{{1, "Alice"}, {2, "Bob"}};
    M moved(std::move(src));

    CHECK(moved.size() == 2u);
    CHECK(moved.contains(1));
    CHECK(moved.contains(2));
}
