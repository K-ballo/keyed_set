// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("swap(keyed_set&) — member swap", "[keyed_set.swap]")
{
    M a{{1, "Alice"}, {2, "Bob"}};
    M b{{10, "X"}};

    a.swap(b);

    CHECK(a.size() == 1u);
    CHECK(a.contains(10));
    CHECK(b.size() == 2u);
    CHECK(b.contains(1));
    CHECK(b.contains(2));
}

TEST_CASE("swap(keyed_set&, keyed_set&) — non-member ADL swap", "[keyed_set.swap]")
{
    M a{{1, "Alice"}};
    M b{{2, "Bob"}, {3, "Carol"}};

    using std::swap;
    swap(a, b);

    CHECK(a.size() == 2u);
    CHECK(b.size() == 1u);
}

TEST_CASE("swap — iterators remain valid after swap", "[keyed_set.swap]")
{
    M a{{1, "Alice"}};
    M b{{2, "Bob"}};

    // Capture iterator into b before the swap
    auto it = b.find(2);
    CHECK(it->id == 2);

    a.swap(b);

    // std::set guarantees iterators are not invalidated by swap;
    // 'it' now points into 'a'
    CHECK(it->id == 2);
    CHECK(a.contains(2));
}
