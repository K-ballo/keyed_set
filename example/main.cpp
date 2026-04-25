// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#include <cassert>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
// Domain types

struct Employee
{
    int         id;
    std::string name;
    double      salary;
};

struct Product
{
    std::string sku;
    std::string description;
    double      price;
};

///////////////////////////////////////////////////////////////////////////////

int main()
{
    // ── Basic usage: integer key ─────────────────────────────────────────────
    {
        eggs::keyed_set<Employee, &Employee::id> roster;

        roster.insert({101, "Alice",  95'000.0});
        roster.insert({102, "Bob",    80'000.0});
        roster.insert({103, "Carol", 110'000.0});

        // Heterogeneous lookup: find by int, no Employee construction needed
        if (auto it = roster.find(102); it != roster.end())
            std::cout << "Found: " << it->name << '\n';

        // Duplicate key is silently rejected
        auto [it, inserted] = roster.insert({101, "Duplicate", 0.0});
        assert(!inserted);
        assert(it->name == "Alice");

        // Iteration is in ascending key order
        std::cout << "Roster (ordered by id):\n";
        for (auto const& e : roster)
            std::cout << "  " << e.id << "  " << e.name << '\n';
    }

    // ── String key ───────────────────────────────────────────────────────────
    {
        eggs::keyed_set<Product, &Product::sku> catalogue;

        catalogue.insert({"SKU-001", "Widget A", 9.99});
        catalogue.insert({"SKU-002", "Widget B", 14.99});
        catalogue.insert({"SKU-003", "Gadget",   49.99});

        assert(catalogue.contains("SKU-002"));

        // lower_bound / upper_bound work on string keys
        auto lo = catalogue.lower_bound("SKU-002");
        auto hi = catalogue.upper_bound("SKU-002");
        assert(lo != catalogue.end() && lo->sku == "SKU-002");
        assert(hi != catalogue.end() && hi->sku == "SKU-003");
    }

#ifdef __cpp_lib_ranges_to_container
    // ── from_range constructor ───────────────────────────────────────────────
    {
        std::vector<Employee> all{
            {1, "Dave",  70'000.0},
            {2, "Eve",   90'000.0},
            {3, "Frank", 60'000.0},
            {4, "Grace", 95'000.0},
        };

        auto high_earners = all
            | std::views::filter([](Employee const& e){ return e.salary >= 90'000.0; });

        eggs::keyed_set<Employee, &Employee::id> top(std::from_range, high_earners);

        assert(top.size() == 2);
        assert(top.contains(2));
        assert(top.contains(4));
    }
#endif

    // ── Iterator-range constructor ───────────────────────────────────────────
    {
        std::vector<Employee> all{
            {1, "Dave",  70'000.0},
            {2, "Eve",   90'000.0},
            {3, "Frank", 60'000.0},
            {4, "Grace", 95'000.0},
        };

        // Filter into a temporary vector first (compatible with all C++23 compilers)
        std::vector<Employee> filtered;
        for (auto const& e : all)
            if (e.salary >= 90'000.0)
                filtered.push_back(e);

        eggs::keyed_set<Employee, &Employee::id> top(filtered.begin(), filtered.end());

        assert(top.size() == 2);
        assert(top.contains(2));
        assert(top.contains(4));
    }

    // ── Node extraction and re-insertion ────────────────────────────────────
    {
        eggs::keyed_set<Employee, &Employee::id> src{{10, "Heidi", 75'000.0}};
        eggs::keyed_set<Employee, &Employee::id> dst;

        auto nh = src.extract(10);
        assert(!nh.empty());
        assert(src.empty());

        auto [pos, ok, node] = dst.insert(std::move(nh));
        assert(ok);
        assert(pos->name == "Heidi");
    }

    // ── merge ────────────────────────────────────────────────────────────────
    {
        eggs::keyed_set<Employee, &Employee::id> team_a{{1, "Ivan",  80'000.0}};
        eggs::keyed_set<Employee, &Employee::id> team_b{
            {2, "Judy",     85'000.0},
            {1, "Conflict",  0.0}
        };

        team_a.merge(team_b);

        // key 1 conflicts: stays in team_b; key 2 moved to team_a
        assert(team_a.size() == 2);
        assert(team_b.size() == 1 && team_b.contains(1));
    }

    std::cout << "All assertions passed.\n";
}
