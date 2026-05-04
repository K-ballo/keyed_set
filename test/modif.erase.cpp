// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("erase(key_type const&) — removes by key, returns count", "[keyed_set.modif]")
{
    M m{{1, "Alice"}, {2, "Bob"}, {3, "Carol"}};

    auto n = m.erase(2);

    CHECK(n == 1u);
    CHECK(m.size() == 2u);
    CHECK(!m.contains(2));
}

TEST_CASE("erase(key_type const&) — missing key returns 0", "[keyed_set.modif]")
{
    M m{{1, "Alice"}};

    auto n = m.erase(99);

    CHECK(n == 0u);
    CHECK(m.size() == 1u);
}

TEST_CASE("erase(key_type const&) — erase only element leaves empty container",
          "[keyed_set.modif]")
{
    M m{{1, "Alice"}};

    auto n = m.erase(1);

    CHECK(n == 1u);
    CHECK(m.empty());
}

TEST_CASE("erase(const_iterator) — removes by iterator, returns next", "[keyed_set.modif]")
{
    M m{{1, "Alice"}, {2, "Bob"}, {3, "Carol"}};

    auto it   = m.find(2);
    auto next = m.erase(it);

    CHECK(next->id == 3);
    CHECK(m.size() == 2u);
    CHECK(!m.contains(2));
}

TEST_CASE("erase(const_iterator) — erase at begin() returns new begin()",
          "[keyed_set.modif]")
{
    M m{{1, "Alice"}, {2, "Bob"}, {3, "Carol"}};

    auto next = m.erase(m.begin());

    CHECK(next == m.begin());
    CHECK(next->id == 2);
    CHECK(m.size() == 2u);
}

TEST_CASE("erase(const_iterator) — erase last element returns end()",
          "[keyed_set.modif]")
{
    M m{{1, "Alice"}, {2, "Bob"}};

    auto last = m.find(2);
    auto next = m.erase(last);

    CHECK(next == m.end());
    CHECK(m.size() == 1u);
}

TEST_CASE("erase(const_iterator) — erase only element returns end()",
          "[keyed_set.modif]")
{
    M m{{1, "Alice"}};

    auto next = m.erase(m.begin());

    CHECK(next == m.end());
    CHECK(m.empty());
}

TEST_CASE("erase(q1, q2) — range erase returns iterator past erased range",
          "[keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}, {4, "D"}};

    auto first = m.find(2);
    auto last  = m.find(4);
    auto after = m.erase(first, last);

    CHECK(after->id == 4);
    CHECK(m.size() == 2u);
    CHECK(m.contains(1));
    CHECK(m.contains(4));
    CHECK(!m.contains(2));
    CHECK(!m.contains(3));
}

TEST_CASE("erase(q1, q2) — erase from begin() to end() clears container",
          "[keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}};

    auto after = m.erase(m.begin(), m.end());

    CHECK(after == m.end());
    CHECK(m.empty());
}

TEST_CASE("erase(q1, q2) — erase empty range [p, p) is a no-op",
          "[keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}};

    auto it    = m.find(2);
    auto after = m.erase(it, it);  // empty range

    CHECK(after->id == 2);         // returns the range start unchanged
    CHECK(m.size() == 3u);
}

TEST_CASE("erase(q1, q2) — erase single-element range [p, next(p))",
          "[keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}};

    auto it   = m.find(2);
    auto next = std::next(it);
    auto after = m.erase(it, next);

    CHECK(after->id == 3);
    CHECK(m.size() == 2u);
    CHECK(!m.contains(2));
}

TEST_CASE("erase(K&&) — transparent erase", "[keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}};

    auto n = m.erase(static_cast<long>(1));

    CHECK(n == 1u);
    CHECK(!m.contains(1));
}
