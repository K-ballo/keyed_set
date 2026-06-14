<!--
Eggs.KeyedSet

Copyright Agustin K-ballo Berge, Fusion Fenix 2026

Distributed under the Boost Software License, Version 1.0. (See
accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-->

# Eggs.KeyedSet

**Eggs.KeyedSet** is a C++23 ordered associative container that stores
unique objects indexed by a designated member field.

## Why

The standard library offers two keyed containers. `std::map<K, V>` pairs
a separate key with its value, duplicating the key if it already lives
inside `V`. `std::set<V>` stores the full value but orders on `V` itself,
which forces a custom comparator every time the ordering is by a member
field rather than the whole value.

`eggs::keyed_set` fills the gap. You name the member once as a non-type
template parameter and get a container that orders by that member, looks
up by its type, and never stores the key separately.

## What it provides

One thing:

- **`<eggs/keyed_set.hpp>`** — A single header that provides
  `eggs::keyed_set`, an ordered associative container backed by
  `std::set` and indexed by a pointer-to-member.

## A quick example

```cpp
#include <eggs/keyed_set.hpp>
#include <string>

struct Employee {
    int         id;
    std::string name;
    double      salary;
};

eggs::keyed_set<Employee, &Employee::id> by_id;

by_id.insert({42, "Alice", 95'000.0});
by_id.insert({7,  "Bob",   80'000.0});

auto it = by_id.find(42);   // lookup by int, not Employee
assert(it->name == "Alice");

for (Employee const& e : by_id)
    // visits in ascending id order: {7, "Bob", ...}, {42, "Alice", ...}
```

Elements are stored in ascending order by `id`. Finding, inserting, and
erasing all accept `int` directly — no `Employee` object needs to be
constructed just for a lookup.

## Requirements

- C++23 compiler (GCC 14+, Clang 18+, MSVC 2022+)
- CMake 3.25+
