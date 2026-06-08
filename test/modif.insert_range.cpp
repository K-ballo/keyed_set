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

TEST_CASE(
    "insert_range(rg) — inserts all elements from a range", "[keyed_set.modif]"
)
{
    std::vector<test::Employee> src{{1, "A"}, {2, "B"}, {3, "C"}};
    M m;
    m.insert_range(src);

    CHECK(m.size() == 3u);
    CHECK(m.contains(1));
    CHECK(m.contains(2));
    CHECK(m.contains(3));
}

TEST_CASE("insert_range(rg) — duplicate keys are ignored", "[keyed_set.modif]")
{
    M m{{1, "Existing"}};
    std::vector<test::Employee> src{{1, "Duplicate"}, {2, "New"}};
    m.insert_range(src);

    CHECK(m.size() == 2u);
    CHECK(m.find(1)->name == "Existing");
}

TEST_CASE(
    "insert_range(rg) — works with a transformed view", "[keyed_set.modif]"
)
{
    auto rng = std::views::iota(10, 14) | std::views::transform([](int i) {
                   return test::Employee{i, "e"};
               });
    M m;
    m.insert_range(rng);

    CHECK(m.size() == 4u);
    for (int i = 10; i < 14; ++i) CHECK(m.contains(i));
}

#else

TEST_CASE("insert_range — not available on this compiler", "[keyed_set.modif]")
{
    SUCCEED(
        "__cpp_lib_ranges_to_container is not defined; insert_range tests "
        "skipped"
    );
}

#endif
