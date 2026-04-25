// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("merge(keyed_set&) — transfers non-conflicting elements", "[keyed_set.modif]")
{
    M a{{1, "Alice"}, {2, "Bob"}};
    M b{{3, "Carol"}, {4, "Dave"}};

    a.merge(b);

    CHECK(a.size() == 4u);
    CHECK(b.empty());
}

TEST_CASE("merge(keyed_set&) — duplicate keys stay in source", "[keyed_set.modif]")
{
    M a{{1, "Alice"}, {2, "Bob"}};
    M b{{2, "Bob-dup"}, {3, "Carol"}};

    a.merge(b);

    // key 2 conflicts: stays in b, key 3 moves to a
    CHECK(a.size() == 3u);
    CHECK(b.size() == 1u);
    CHECK(b.contains(2));
    CHECK(a.find(2)->name == "Bob");  // original untouched
}

TEST_CASE("merge(keyed_set&&) — rvalue overload", "[keyed_set.modif]")
{
    M a{{1, "Alice"}};
    M b{{2, "Bob"}};

    a.merge(std::move(b));

    CHECK(a.size() == 2u);
}

TEST_CASE("merge — pointers/references to transferred elements remain valid",
          "[keyed_set.modif]")
{
    M a{{1, "Alice"}};
    M b{{2, "Bob"}};

    // Capture a pointer before the merge
    auto const* ptr = &(*b.find(2));

    a.merge(b);

    // The element is now in `a`; the pointer must still be valid and refer
    // to the same object ([associative.reqmts.general]/p115).
    CHECK(ptr->id == 2);
    CHECK(ptr->name == "Bob");
}
