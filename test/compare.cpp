// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <functional>
#include <limits>
#include <string>
#include <type_traits>

#include "fixture.hpp"
#include <catch.hpp>

// ── Default Compare = std::less<key_type> ───────────────────────────────────

using Asc = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE(
    "keyed_set — default Compare is std::less<key_type>", "[keyed_set.compare]"
)
{
    static_assert(std::is_same_v<Asc::key_compare, std::less<int>>);
}

TEST_CASE(
    "keyed_set — default Compare iterates in ascending order",
    "[keyed_set.compare]"
)
{
    Asc m{{3, "C"}, {1, "A"}, {2, "B"}};

    int prev = -1;
    for (auto const& e : m) {
        CHECK(e.id > prev);
        prev = e.id;
    }
}

// ── Custom Compare = std::greater<key_type> ──────────────────────────────────

using Desc =
    eggs::keyed_set<test::Employee, &test::Employee::id, std::greater<int>>;

TEST_CASE(
    "keyed_set — std::greater<> yields descending order", "[keyed_set.compare]"
)
{
    Desc m{{3, "C"}, {1, "A"}, {2, "B"}};

    int prev = std::numeric_limits<int>::max();
    for (auto const& e : m) {
        CHECK(e.id < prev);
        prev = e.id;
    }
}

TEST_CASE("keyed_set — std::greater<> find by key_type", "[keyed_set.compare]")
{
    Desc m{{1, "Alice"}, {2, "Bob"}, {3, "Carol"}};

    auto it = m.find(2);
    REQUIRE(it != m.end());
    CHECK(it->name == "Bob");
}

TEST_CASE(
    "keyed_set — std::greater<> lower_bound / upper_bound are reversed",
    "[keyed_set.compare]"
)
{
    Desc m{{1, "A"}, {2, "B"}, {3, "C"}};

    // With greater<>, lower_bound(2) finds first element not greater-than 2,
    // i.e. the first element <= 2 in descending order: that's 2 itself.
    auto lo = m.lower_bound(2);
    REQUIRE(lo != m.end());
    CHECK(lo->id == 2);

    // upper_bound(2) finds first element greater-than(2) in the greater<> sense,
    // i.e. the first element < 2: that's 1.
    auto hi = m.upper_bound(2);
    REQUIRE(hi != m.end());
    CHECK(hi->id == 1);
}

// ── Transparent Compare ───────────────────────────────────────────────────────

TEST_CASE(
    "keyed_set — std::less<int> (non-transparent) key_compare has no "
    "is_transparent",
    "[keyed_set.compare]"
)
{
    // key_compare is Compare itself; std::less<int> is not transparent.
    static_assert(!test::transparent<Asc::key_compare>);
    // value_compare (the projecting adaptor) always has is_transparent.
    static_assert(test::transparent<Asc::value_compare>);
}

TEST_CASE(
    "keyed_set — std::less<void> (transparent) propagates is_transparent",
    "[keyed_set.compare]"
)
{
    using Trans =
        eggs::keyed_set<test::Employee, &test::Employee::id, std::less<>>;
    static_assert(test::transparent<Trans::key_compare>);
}

// ── key_comp() observer ──────────────────────────────────────────────────────

TEST_CASE(
    "keyed_set — key_comp() returns the Compare object", "[keyed_set.compare]"
)
{
    Desc m;
    auto cmp = m.key_comp();
    static_assert(std::is_same_v<decltype(cmp), std::greater<int>>);
    CHECK(cmp(3, 2)); // 3 > 2
    CHECK(!cmp(2, 3));
}

// ── String key with custom comparator ────────────────────────────────────────

TEST_CASE(
    "keyed_set — string key with case-insensitive comparator",
    "[keyed_set.compare]"
)
{
    struct ILess
    {
        using is_transparent = void;

        bool operator()(std::string const& a, std::string const& b) const
        {
            return std::lexicographical_compare(
                a.begin(), a.end(), b.begin(), b.end(), [](char x, char y) {
                    return std::tolower(x) < std::tolower(y);
                }
            );
        }
    };

    using CISet = eggs::keyed_set<test::Widget, &test::Widget::code, ILess>;

    CISet m;
    m.insert({"SKU-A", 1.0});
    m.insert({"sku-b", 2.0});

    // Case-insensitive find
    CHECK(m.contains("SKU-A"));
    CHECK(m.contains("sku-a")); // same key under ILess
    CHECK(m.size() == 2u);
}

// ── Stateful comparator ───────────────────────────────────────────────────────

TEST_CASE(
    "keyed_set — stateful comparator is stored and accessible",
    "[keyed_set.compare]"
)
{
    struct ThresholdLess
    {
        int threshold;

        explicit ThresholdLess(int t)
            : threshold(t)
        {
        }

        bool operator()(int a, int b) const { return a < b; }
    };

    using TM =
        eggs::keyed_set<test::Employee, &test::Employee::id, ThresholdLess>;

    ThresholdLess cmp(42);
    TM m(cmp);

    m.insert({1, "Alice"});
    m.insert({2, "Bob"});

    CHECK(m.key_comp().threshold == 42);
    CHECK(m.size() == 2u);
}

TEST_CASE(
    "keyed_set — stateful comparator is copied with the container",
    "[keyed_set.compare]"
)
{
    struct TaggedLess
    {
        int tag;

        explicit TaggedLess(int t)
            : tag(t)
        {
        }

        bool operator()(int a, int b) const { return a < b; }
    };

    using TM = eggs::keyed_set<test::Employee, &test::Employee::id, TaggedLess>;

    TM src(TaggedLess{99});
    src.insert({1, "Alice"});

    TM copy(src);
    CHECK(copy.key_comp().tag == 99);
    CHECK(copy.size() == 1u);
}

TEST_CASE(
    "keyed_set — stateful comparator is moved with the container",
    "[keyed_set.compare]"
)
{
    struct TaggedLess
    {
        int tag;

        explicit TaggedLess(int t)
            : tag(t)
        {
        }

        bool operator()(int a, int b) const { return a < b; }
    };

    using TM = eggs::keyed_set<test::Employee, &test::Employee::id, TaggedLess>;

    TM src(TaggedLess{77});
    src.insert({1, "Alice"});

    TM moved(std::move(src));
    CHECK(moved.key_comp().tag == 77);
    CHECK(moved.size() == 1u);
}

// ── Compare + Allocator combination ──────────────────────────────────────────

// Counting allocator defined at namespace scope so it can have a converting
// constructor template (local structs cannot have template members).
template <typename T>
struct compare_test_alloc
{
    using value_type = T;
    int* count;

    explicit compare_test_alloc(int& c)
        : count(&c)
    {
    }

    template <typename U>
    compare_test_alloc(compare_test_alloc<U> const& o) noexcept
        : count(o.count)
    {
    }

    T* allocate(std::size_t n)
    {
        ++(*count);
        return std::allocator<T>{}.allocate(n);
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        std::allocator<T>{}.deallocate(p, n);
    }

    friend bool operator==(
        compare_test_alloc const& a, compare_test_alloc const& b
    ) noexcept
    {
        return a.count == b.count;
    }
};

using CA = eggs::keyed_set<
    test::Employee,
    &test::Employee::id,
    std::greater<int>,
    compare_test_alloc<test::Employee>>;

TEST_CASE(
    "keyed_set — custom Compare and custom Allocator together",
    "[keyed_set.compare]"
)
{
    static_assert(std::is_same_v<CA::key_compare, std::greater<int>>);
    static_assert(
        std::is_same_v<CA::allocator_type, compare_test_alloc<test::Employee>>
    );

    int alloc_count = 0;
    CA m(std::greater<int>{}, compare_test_alloc<test::Employee>{alloc_count});
    m.insert({3, "Carol"});
    m.insert({1, "Alice"});
    m.insert({2, "Bob"});

    // Ordering is descending (greater<>)
    int prev = std::numeric_limits<int>::max();
    for (auto const& e : m) {
        CHECK(e.id < prev);
        prev = e.id;
    }

    // Custom allocator was actually used
    CHECK(alloc_count >= 3);
}
