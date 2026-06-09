// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <compare>

#include "fixture.hpp"
#include <catch.hpp>

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

namespace {
struct Priority
{
    int value;

    friend std::weak_ordering operator<=>(Priority const& a, Priority const& b)
    {
        bool a_neg = a.value < 0;
        bool b_neg = b.value < 0;
        if (a_neg && b_neg) return std::weak_ordering::equivalent;
        return a.value <=> b.value;
    }
};

struct Task
{
    Priority priority;
    std::string name;

    friend std::weak_ordering operator<=>(Task const& a, Task const& b)
    {
        if (auto c = a.priority <=> b.priority; c != 0) return c;
        return std::weak_ordering(a.name <=> b.name);
    }
};
} // namespace

TEST_CASE("operator<=> — weak_ordering key type", "[keyed_set.cmp]")
{
    struct PriorityLess
    {
        bool operator()(Priority const& a, Priority const& b) const
        {
            return std::is_lt(a <=> b);
        }
    };

    using TM = eggs::keyed_set<Task, &Task::priority, PriorityLess>;

    TM a{{{1}, "high"}};
    TM b{{{2}, "higher"}};
    TM c{{{1}, "high"}};

    CHECK(std::is_lt(a <=> b));
    CHECK(std::is_gt(b <=> a));
    CHECK(std::is_eq(a <=> c));
}

TEST_CASE(
    "operator== — same keys different values are not equal", "[keyed_set.cmp]"
)
{
    M a{{1, "Alice"}};
    M b{{1, "Bob"}}; // same key, different name
    CHECK(!(a == b));
}

TEST_CASE(
    "operator<=> — full value comparison, not just key", "[keyed_set.cmp]"
)
{
    M a{{1, "Alice"}};
    M b{{1, "Bob"}}; // same key, "Alice" < "Bob" lexicographically
    CHECK(std::is_lt(a <=> b));
}
