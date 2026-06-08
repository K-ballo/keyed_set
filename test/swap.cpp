// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include "fixture.hpp"
#include <catch.hpp>

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

TEST_CASE(
    "swap(keyed_set&, keyed_set&) — non-member ADL swap", "[keyed_set.swap]"
)
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

// ── Swap with allocators ──────────────────────────────────────────────────────

// std::allocator<T> has propagate_on_container_swap = false but is always
// equal, so swap is well-defined. We verify that swap works correctly when
// both containers use the same allocator instance (the common case).
TEST_CASE(
    "swap — equal allocators, elements exchanged correctly", "[keyed_set.swap]"
)
{
    using M2 = eggs::keyed_set<test::Employee, &test::Employee::id>;

    M2 a{{1, "Alice"}, {2, "Bob"}};
    M2 b{{10, "Xavier"}, {20, "Yara"}, {30, "Zara"}};

    a.swap(b);

    REQUIRE(a.size() == 3u);
    CHECK(a.contains(10));
    CHECK(a.contains(20));
    CHECK(a.contains(30));

    REQUIRE(b.size() == 2u);
    CHECK(b.contains(1));
    CHECK(b.contains(2));
}

// A stateful allocator with propagate_on_container_swap = true
template <typename T>
struct swap_tracking_alloc
{
    using value_type = T;
    using propagate_on_container_swap = std::true_type;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;

    int id;

    explicit swap_tracking_alloc(int i)
        : id(i)
    {
    }

    template <typename U>
    swap_tracking_alloc(swap_tracking_alloc<U> const& o) noexcept
        : id(o.id)
    {
    }

    T* allocate(std::size_t n) { return std::allocator<T>{}.allocate(n); }

    void deallocate(T* p, std::size_t n) noexcept
    {
        std::allocator<T>{}.deallocate(p, n);
    }

    friend bool operator==(
        swap_tracking_alloc const& a, swap_tracking_alloc const& b
    ) noexcept
    {
        return a.id == b.id;
    }
};

TEST_CASE(
    "swap — propagate_on_container_swap=true swaps allocators",
    "[keyed_set.swap]"
)
{
    using A = swap_tracking_alloc<test::Employee>;
    using MS =
        eggs::keyed_set<test::Employee, &test::Employee::id, std::less<int>, A>;

    MS a(A{1});
    a.insert({1, "Alice"});

    MS b(A{2});
    b.insert({2, "Bob"});

    a.swap(b);

    // Elements exchanged
    CHECK(a.contains(2));
    CHECK(b.contains(1));

    // Allocators exchanged (propagate_on_container_swap = true)
    CHECK(a.get_allocator().id == 2);
    CHECK(b.get_allocator().id == 1);
}
