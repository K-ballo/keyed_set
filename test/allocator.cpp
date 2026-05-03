// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

#include <memory>
#include <scoped_allocator>
#include <type_traits>
#include <vector>

// ── Tracking allocator ───────────────────────────────────────────────────────

template <typename T>
struct tracking_allocator
{
    using value_type = T;

    int* alloc_count;
    int* dealloc_count;

    tracking_allocator(int& ac, int& dc)
        : alloc_count(&ac), dealloc_count(&dc)
    {}

    template <typename U>
    tracking_allocator(tracking_allocator<U> const& other) noexcept
        : alloc_count(other.alloc_count)
        , dealloc_count(other.dealloc_count)
    {}

    T* allocate(std::size_t n)
    {
        ++(*alloc_count);
        return std::allocator<T>{}.allocate(n);
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        ++(*dealloc_count);
        std::allocator<T>{}.deallocate(p, n);
    }

    friend bool operator==(tracking_allocator const& a,
                           tracking_allocator const& b) noexcept
    { return a.alloc_count == b.alloc_count; }
};

using TrackingAlloc = tracking_allocator<test::Employee>;
using M = eggs::keyed_set<test::Employee, &test::Employee::id,
                          std::less<int>, TrackingAlloc>;

// ── allocator_type reflects the template parameter ───────────────────────────

TEST_CASE("keyed_set — allocator_type reflects template parameter",
          "[keyed_set.alloc]")
{
    static_assert(std::is_same_v<M::allocator_type, TrackingAlloc>);
}

TEST_CASE("keyed_set — default allocator_type is std::allocator<value_type>",
          "[keyed_set.alloc]")
{
    using Default = eggs::keyed_set<test::Employee, &test::Employee::id>;
    static_assert(std::is_same_v<Default::allocator_type,
                                 std::allocator<test::Employee>>);
}

// ── get_allocator() ──────────────────────────────────────────────────────────

TEST_CASE("keyed_set — get_allocator() returns the stored allocator",
          "[keyed_set.alloc]")
{
    int ac = 0, dc = 0;
    TrackingAlloc alloc(ac, dc);
    M m(alloc);

    CHECK(m.get_allocator() == alloc);
}

// ── Allocator is actually used ───────────────────────────────────────────────

TEST_CASE("keyed_set — custom allocator is used for node allocation",
          "[keyed_set.alloc]")
{
    int ac = 0, dc = 0;
    {
        M m(TrackingAlloc{ac, dc});
        m.insert({1, "Alice"});
        m.insert({2, "Bob"});
        m.insert({3, "Carol"});
        CHECK(ac >= 3);  // at least one allocation per node
    }
    // All nodes freed on destruction
    CHECK(dc == ac);
}

// ── Allocator-extended constructors ──────────────────────────────────────────

TEST_CASE("keyed_set(allocator) — allocator-only constructor",
          "[keyed_set.alloc]")
{
    int ac = 0, dc = 0;
    M m(TrackingAlloc{ac, dc});

    CHECK(m.empty());
    CHECK(m.get_allocator() == TrackingAlloc{ac, dc});
}

TEST_CASE("keyed_set(compare, allocator) — compare + allocator constructor",
          "[keyed_set.alloc]")
{
    int ac = 0, dc = 0;
    M m(std::less<int>{}, TrackingAlloc{ac, dc});

    CHECK(m.empty());
}

TEST_CASE("keyed_set(i, j, allocator) — range + allocator constructor",
          "[keyed_set.alloc]")
{
    int ac = 0, dc = 0;
    std::vector<test::Employee> src{{1, "A"}, {2, "B"}, {3, "C"}};
    M m(src.begin(), src.end(), TrackingAlloc{ac, dc});

    REQUIRE(m.size() == 3u);
    CHECK(ac >= 3);
}

TEST_CASE("keyed_set(il, allocator) — initializer-list + allocator constructor",
          "[keyed_set.alloc]")
{
    int ac = 0, dc = 0;
    M m({{1, "A"}, {2, "B"}}, TrackingAlloc{ac, dc});

    REQUIRE(m.size() == 2u);
    CHECK(ac >= 2);
}

TEST_CASE("keyed_set(const keyed_set&, allocator) — copy + allocator constructor",
          "[keyed_set.alloc]")
{
    int ac = 0, dc = 0;
    M src({{1, "A"}, {2, "B"}}, TrackingAlloc{ac, dc});

    int ac2 = 0, dc2 = 0;
    M copy(src, TrackingAlloc{ac2, dc2});

    CHECK(copy == src);
    CHECK(ac2 >= 2);
}

TEST_CASE("keyed_set(keyed_set&&, allocator) — move + allocator constructor",
          "[keyed_set.alloc]")
{
    int ac = 0, dc = 0;
    M src({{1, "A"}, {2, "B"}}, TrackingAlloc{ac, dc});

    M moved(std::move(src), TrackingAlloc{ac, dc});

    CHECK(moved.size() == 2u);
    CHECK(moved.contains(1));
    CHECK(moved.contains(2));
}
