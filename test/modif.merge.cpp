// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

#include <algorithm>
#include <vector>

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

TEST_CASE("merge(keyed_set&) — all keys conflict, source unchanged", "[keyed_set.modif]")
{
    M a{{1, "A"}, {2, "B"}, {3, "C"}};
    M b{{1, "X"}, {2, "Y"}, {3, "Z"}};

    a.merge(b);

    CHECK(a.size() == 3u);
    CHECK(b.size() == 3u);  // nothing moved
    // destination values unchanged
    CHECK(a.find(1)->name == "A");
    CHECK(a.find(2)->name == "B");
    CHECK(a.find(3)->name == "C");
}

TEST_CASE("merge(keyed_set&) — source becomes empty when no conflicts",
          "[keyed_set.modif]")
{
    M a;
    M b{{1, "A"}, {2, "B"}, {3, "C"}};

    a.merge(b);

    CHECK(a.size() == 3u);
    CHECK(b.empty());
}

TEST_CASE("merge(keyed_set&&) — rvalue overload", "[keyed_set.modif]")
{
    M a{{1, "Alice"}};
    M b{{2, "Bob"}};

    a.merge(std::move(b));

    CHECK(a.size() == 2u);
}

TEST_CASE("merge — result is still ordered", "[keyed_set.modif]")
{
    M a{{5, "E"}, {3, "C"}, {1, "A"}};
    M b{{4, "D"}, {2, "B"}};

    a.merge(b);

    std::vector<int> ids;
    for (auto const& e : a)
        ids.push_back(e.id);

    CHECK(std::is_sorted(ids.begin(), ids.end()));
    CHECK(ids == std::vector<int>{1, 2, 3, 4, 5});
}

TEST_CASE("merge — pointers to transferred elements remain valid",
          "[keyed_set.modif]")
{
    M a{{1, "Alice"}};
    M b{{2, "Bob"}};

    auto const* ptr = &(*b.find(2));

    a.merge(b);

    // The element is now in `a`; the pointer must still be valid
    // ([associative.reqmts.general]/p115).
    CHECK(ptr->id == 2);
    CHECK(ptr->name == "Bob");
}

TEST_CASE("merge — iterators to transferred elements remain valid",
          "[keyed_set.modif]")
{
    M a{{1, "Alice"}};
    M b{{2, "Bob"}, {3, "Carol"}};

    // Capture iterators into b before the merge
    auto it2 = b.find(2);
    auto it3 = b.find(3);
    REQUIRE(it2 != b.end());
    REQUIRE(it3 != b.end());

    a.merge(b);

    // Iterators must remain valid and dereferenceable in a
    // ([associative.reqmts.general]/p115).
    CHECK(it2->id == 2);
    CHECK(it3->id == 3);
    // They now belong to a
    CHECK(a.contains(2));
    CHECK(a.contains(3));
}

TEST_CASE("merge — partial transfer preserves order in both containers",
          "[keyed_set.modif]")
{
    M a{{1, "A"}, {3, "C"}, {5, "E"}};
    M b{{2, "B"}, {3, "C-dup"}, {4, "D"}};

    a.merge(b);

    // a should contain 1,2,3,4,5 in order
    std::vector<int> a_ids;
    for (auto const& e : a) a_ids.push_back(e.id);
    CHECK(std::is_sorted(a_ids.begin(), a_ids.end()));

    // b should only contain the conflicting key 3
    CHECK(b.size() == 1u);
    CHECK(b.contains(3));
}
