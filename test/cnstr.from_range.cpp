// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>

#ifdef __cpp_lib_ranges_to_container

#    include <eggs/keyed_set.hpp>

#    include <ranges>
#    include <vector>

#    include "fixture.hpp"

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("keyed_set(from_range, rg) — range constructor", "[keyed_set.cnstr]")
{
    std::vector<test::Employee> src{{10, "Dave"}, {20, "Eve"}, {30, "Frank"}};

    M m(std::from_range, src);

    REQUIRE(m.size() == 3u);
    CHECK(m.contains(10));
    CHECK(m.contains(20));
    CHECK(m.contains(30));
}

TEST_CASE("keyed_set(from_range, rg) — works with views", "[keyed_set.cnstr]")
{
    std::vector<test::Employee> src{{1, "A"}, {2, "B"}, {3, "C"}, {4, "D"}};
    auto evens = src | std::views::filter([](test::Employee const& e) {
                     return e.id % 2 == 0;
                 });

    M m(std::from_range, evens);

    REQUIRE(m.size() == 2u);
    CHECK(m.contains(2));
    CHECK(m.contains(4));
    CHECK(!m.contains(1));
}

#else

TEST_CASE(
    "keyed_set(from_range) — not available on this compiler",
    "[keyed_set.cnstr]"
)
{
    SUCCEED(
        "__cpp_lib_ranges_to_container is not defined; from_range tests skipped"
    );
}

#endif
