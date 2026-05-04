// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

#include <compare>

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("operator== — equal containers", "[keyed_set.cmp]")
{
    M a{{1, "Alice"}, {2, "Bob"}};
    M b{{1, "Alice"}, {2, "Bob"}};
    CHECK(a == b);
}

TEST_CASE("operator== — different size", "[keyed_set.cmp]")
{
    M a{{1, "Alice"}};
    M b{{1, "Alice"}, {2, "Bob"}};
    CHECK(!(a == b));
}

TEST_CASE("operator== — same size, different elements", "[keyed_set.cmp]")
{
    M a{{1, "Alice"}};
    M b{{2, "Bob"}};
    CHECK(!(a == b));
}

TEST_CASE("operator<=> — less than", "[keyed_set.cmp]")
{
    M a{{1, "Alice"}};
    M b{{2, "Bob"}};
    // Avoid passing std::strong_ordering directly to Catch's expression
    // decomposer: MSVC's strong_ordering comparison against int uses a
    // consteval constructor that Catch2 v2 cannot handle.
    CHECK(std::is_lt(a <=> b));
    CHECK(std::is_gt(b <=> a));
}

TEST_CASE("operator<=> — equal", "[keyed_set.cmp]")
{
    M a{{1, "Alice"}};
    M b{{1, "Alice"}};
    CHECK(std::is_eq(a <=> b));
}

TEST_CASE("operator<= / >= / < / > — synthesised from <=>", "[keyed_set.cmp]")
{
    M a{{1, "A"}};
    M b{{2, "B"}};

    CHECK(a < b);
    CHECK(a <= b);
    CHECK(b > a);
    CHECK(b >= a);
    CHECK(!(a > b));
    CHECK(!(b < a));
}

// ── Non-strong ordering ───────────────────────────────────────────────────────

namespace
{
    // A key type whose operator<=> returns weak_ordering
    struct Priority
    {
        int value;
        // Intentionally weak: treat all negatives as equivalent
        friend std::weak_ordering operator<=>(Priority const& a, Priority const& b)
        {
            bool a_neg = a.value < 0;
            bool b_neg = b.value < 0;
            if (a_neg && b_neg) return std::weak_ordering::equivalent;
            return a.value <=> b.value;
        }
    };

    struct Task { Priority priority; std::string name; };
}

TEST_CASE("operator<=> — weak_ordering key type yields weak_ordering result",
          "[keyed_set.cmp]")
{
    // std::less on a weak_ordering type: need a comparator that uses <
    struct PriorityLess {
        bool operator()(Priority const& a, Priority const& b) const
        { return std::is_lt(a <=> b); }
    };

    using TM = eggs::keyed_set<Task, &Task::priority, PriorityLess>;

    TM a{{{1}, "high"}};
    TM b{{{2}, "higher"}};
    TM c{{{1}, "high"}};

    CHECK(std::is_lt(a <=> b));
    CHECK(std::is_gt(b <=> a));
    CHECK(std::is_eq(a <=> c));
}
