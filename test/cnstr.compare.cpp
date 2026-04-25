// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

TEST_CASE("keyed_set(c) — comparator constructor", "[keyed_set.cnstr]")
{
    using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

    M::key_compare cmp;
    M m(cmp);

    CHECK(m.empty());
}
