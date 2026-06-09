// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_KEYED_SET_TEST_FIXTURE_HPP
#define EGGS_KEYED_SET_TEST_FIXTURE_HPP

#include <string>

namespace test {
///////////////////////////////////////////////////////////////////////////
//! Concept that checks whether a type defines `is_transparent`.
//! Used in static_assert to avoid hard errors on non-dependent types.
template <typename T>
concept transparent = requires { typename T::is_transparent; };

///////////////////////////////////////////////////////////////////////////
struct Employee
{
    int id;
    std::string name;

    friend bool operator==(Employee const&, Employee const&) = default;
    friend auto operator<=>(Employee const&, Employee const&) = default;
};

///////////////////////////////////////////////////////////////////////////
//! A type whose key member has a non-trivial type, for broader coverage.
struct Widget
{
    std::string code;
    double weight = 0.0;

    friend bool operator==(Widget const&, Widget const&) = default;
    friend auto operator<=>(Widget const&, Widget const&) = default;
};

///////////////////////////////////////////////////////////////////////////
//! Transparent lookup key: a type convertible-from but not equal-to int,
//! used to exercise heterogeneous lookup paths.
struct EmployeeId
{
    int value;

    explicit EmployeeId(int v)
        : value(v)
    {
    }

    friend bool operator<(EmployeeId const& a, int b) { return a.value < b; }

    friend bool operator<(int a, EmployeeId const& b) { return a < b.value; }

    friend bool operator<(EmployeeId const& a, EmployeeId const& b)
    {
        return a.value < b.value;
    }
};

} // namespace test

#endif // EGGS_KEYED_SET_TEST_FIXTURE_HPP
