// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <array>

#include "fixture.hpp"
#include <catch.hpp>

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("insert(value_type const&) — lvalue", "[keyed_set.modif]")
{
    M m;
    test::Employee e{42, "Alice"};
    auto [it, ok] = m.insert(e);

    REQUIRE(ok);
    CHECK(it->id == 42);
}

TEST_CASE("insert(value_type&&) — rvalue", "[keyed_set.modif]")
{
    M m;
    auto [it, ok] = m.insert(test::Employee{7, "Bob"});

    REQUIRE(ok);
    CHECK(it->id == 7);
}

TEST_CASE("insert(hint, value_type const&) — hint", "[keyed_set.modif]")
{
    M m{{1, "A"}, {3, "C"}};
    test::Employee e{2, "B"};
    auto it = m.insert(m.end(), e);

    CHECK(it->id == 2);
    CHECK(m.size() == 3u);
}

TEST_CASE("insert(i, j) — iterator range", "[keyed_set.modif]")
{
    std::array<test::Employee, 3> arr{{{10, "X"}, {20, "Y"}, {30, "Z"}}};
    M m;
    m.insert(arr.begin(), arr.end());

    CHECK(m.size() == 3u);
}

TEST_CASE("insert(il) — initializer list", "[keyed_set.modif]")
{
    M m;
    m.insert({{1, "A"}, {2, "B"}, {3, "C"}});

    CHECK(m.size() == 3u);
}

TEST_CASE("insert(node_type&&) — node handle", "[keyed_set.modif]")
{
    M src{{99, "Node"}};
    auto nh = src.extract(99);

    M dst;
    auto result = dst.insert(std::move(nh));

    CHECK(result.inserted);
    CHECK(result.position->id == 99);
    CHECK(dst.size() == 1u);
}
