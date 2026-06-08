// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <iterator>
#include <type_traits>

#include "fixture.hpp"
#include <catch.hpp>

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

// ── [associative.reqmts.general] required type members ──────────────────────

TEST_CASE("keyed_set — key_type", "[keyed_set.types]")
{
    static_assert(std::is_same_v<M::key_type, int>);
}

TEST_CASE("keyed_set — value_type", "[keyed_set.types]")
{
    static_assert(std::is_same_v<M::value_type, test::Employee>);
}

TEST_CASE("keyed_set — key_compare is CopyConstructible", "[keyed_set.types]")
{
    static_assert(std::is_copy_constructible_v<M::key_compare>);
}

TEST_CASE("keyed_set — value_compare", "[keyed_set.types]")
{
    // key_compare is the user-supplied Compare;
    // value_compare is the projecting adaptor — they are distinct.
    static_assert(!std::is_same_v<M::key_compare, M::value_compare>);
    static_assert(std::is_same_v<M::key_compare, std::less<int>>);
}

TEST_CASE("keyed_set — iterator is constant bidirectional", "[keyed_set.types]")
{
    static_assert(std::is_same_v<M::iterator, M::const_iterator>);
    static_assert(std::bidirectional_iterator<M::iterator>);
    // Because iterator == const_iterator, all lookup and iteration functions
    // are const-qualified; there are no separate non-const overloads.
    static_assert(
        std::is_const_v<
            std::remove_reference_t<decltype(*std::declval<M::iterator>())>>
    );
}

TEST_CASE(
    "keyed_set — reverse_iterator is constant bidirectional",
    "[keyed_set.types]"
)
{
    static_assert(
        std::is_same_v<M::reverse_iterator, M::const_reverse_iterator>
    );
}

TEST_CASE(
    "keyed_set — node_type and insert_return_type exist", "[keyed_set.types]"
)
{
    static_assert(requires { typename M::node_type; });
    static_assert(requires { typename M::insert_return_type; });
}
