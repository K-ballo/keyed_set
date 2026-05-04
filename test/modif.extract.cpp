// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <catch.hpp>

#include "fixture.hpp"

using M = eggs::keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("extract(key_type const&) — removes and returns the node", "[keyed_set.modif]")
{
    M m{{1, "Alice"}, {2, "Bob"}};

    auto nh = m.extract(1);

    REQUIRE(!nh.empty());
    CHECK(nh.value().id == 1);
    CHECK(nh.value().name == "Alice");
    CHECK(m.size() == 1u);
    CHECK(!m.contains(1));
}

TEST_CASE("extract(key_type const&) — missing key returns empty node", "[keyed_set.modif]")
{
    M m{{1, "Alice"}};

    auto nh = m.extract(99);

    CHECK(nh.empty());
    CHECK(m.size() == 1u);
}

TEST_CASE("extract(const_iterator) — removes via iterator", "[keyed_set.modif]")
{
    M m{{1, "Alice"}, {2, "Bob"}};

    auto it = m.find(2);
    REQUIRE(it != m.end());

    auto nh = m.extract(it);

    REQUIRE(!nh.empty());
    CHECK(nh.value().id == 2);
    CHECK(m.size() == 1u);
}

TEST_CASE("extract / re-insert round-trip — success", "[keyed_set.modif]")
{
    M src{{5, "Eve"}};
    auto nh = src.extract(5);
    CHECK(src.empty());

    M dst;
    auto [pos, inserted, node] = dst.insert(std::move(nh));

    CHECK(inserted);
    CHECK(pos->id == 5);
    CHECK(pos->name == "Eve");
    CHECK(dst.size() == 1u);
    CHECK(node.empty());   // node handle consumed on success
}

TEST_CASE("extract / re-insert round-trip — duplicate key not inserted",
          "[keyed_set.modif]")
{
    M src{{5, "Eve"}};
    M dst{{5, "Existing"}};

    auto nh = src.extract(5);
    auto [pos, inserted, node] = dst.insert(std::move(nh));

    CHECK(!inserted);
    CHECK(pos->name == "Existing");  // existing element untouched
    CHECK(!node.empty());            // node handle returned unconsumed
    CHECK(node.value().id == 5);
    CHECK(node.value().name == "Eve");
    CHECK(dst.size() == 1u);
}

TEST_CASE("extract / re-insert with hint — success", "[keyed_set.modif]")
{
    M src{{3, "Carol"}};
    M dst{{1, "Alice"}, {5, "Eve"}};

    auto nh = src.extract(3);
    // hint points past the correct insertion position
    auto it = dst.insert(dst.find(5), std::move(nh));

    CHECK(it->id == 3);
    CHECK(dst.size() == 3u);
    CHECK(dst.contains(3));
}

TEST_CASE("extract preserves element value through node handle", "[keyed_set.modif]")
{
    M m{{42, "Answer"}};
    auto nh = m.extract(42);

    // Value must be accessible and correct while in the node handle
    REQUIRE(!nh.empty());
    CHECK(nh.value().id == 42);
    CHECK(nh.value().name == "Answer");

    // Modify through the node handle before reinsertion
    nh.value().name = "Modified";

    M dst;
    auto [pos, inserted, node] = dst.insert(std::move(nh));
    CHECK(inserted);
    CHECK(pos->name == "Modified");
}
