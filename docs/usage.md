<!--
Eggs.KeyedSet

Copyright Agustin K-ballo Berge, Fusion Fenix 2026

Distributed under the Boost Software License, Version 1.0. (See
accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-->

# Usage

## Inserting and looking up

Declare the container with the element type and the member to key on.
Pass full values to `insert` or `emplace`; look up by the key type, not
by the whole element.

```cpp
#include <eggs/keyed_set.hpp>
#include <string>

struct Employee {
    int         id;
    std::string name;
    double      salary;
};

eggs::keyed_set<Employee, &Employee::id> employees;

employees.insert({42, "Alice", 95'000.0});
employees.insert({7,  "Bob",   80'000.0});
employees.emplace(99, "Carol", 70'000.0);

auto it = employees.find(42);       // accepts int, not Employee
if (it != employees.end())
    // it->name == "Alice"

bool present = employees.contains(7);   // true
```

`find`, `contains`, `count`, `lower_bound`, `upper_bound`, and
`equal_range` all accept `int` directly — no `Employee` is constructed
for a lookup.

---

## Iterating in order

Iteration visits elements in ascending key order. Iterators are always
`const` — the key member of a stored element cannot be modified through
an iterator.

```cpp
for (Employee const& e : employees) {
    // visits: {7, "Bob", ...}, {42, "Alice", ...}, {99, "Carol", ...}
}
```

---

## Transparent (heterogeneous) lookup

When the comparator is transparent — for example `std::less<>` — lookup
accepts any type comparable to `key_type` without first converting it.

```cpp
eggs::keyed_set<Employee, &Employee::id, std::less<>> employees;
employees.insert({42, "Alice", 95'000.0});

// Look up by long without converting to int
long query = 42L;
auto it = employees.find(query);
```

Without a transparent comparator, only `key_type` is accepted in lookup;
the generic overloads do not participate in overload resolution.

---

## Erasing elements

Erase by key value, by iterator, or by range:

```cpp
employees.erase(42);                            // by key — returns count (0 or 1)
employees.erase(employees.find(7));             // by iterator
employees.erase(
    employees.lower_bound(10),
    employees.upper_bound(90));                 // by range
```

---

## Erasing with a predicate

`erase_if` removes all elements for which the predicate returns `true`
and returns the number of elements erased.

```cpp
// Remove all employees whose salary is below 75000
eggs::erase_if(employees, [](Employee const& e) {
    return e.salary < 75'000.0;
});
```

---

## Node handles

Elements can be extracted from one container and inserted into another
without copying the stored value.

```cpp
eggs::keyed_set<Employee, &Employee::id> a = {{1, "Dave", 60'000.0}};
eggs::keyed_set<Employee, &Employee::id> b;

auto node = a.extract(1);       // removes from a, returns node handle
if (!node.empty())
    b.insert(std::move(node));  // inserts into b without copying Employee
```

---

## Constructing from a range

Pass an iterator range, a `std::ranges` range, or an initializer list:

```cpp
std::vector<Employee> vec = {{1, "Dave", 60'000.0}, {2, "Eve", 65'000.0}};

// Iterator-range constructor
eggs::keyed_set<Employee, &Employee::id> a(vec.begin(), vec.end());

// from_range constructor (C++23)
eggs::keyed_set<Employee, &Employee::id> b(std::from_range, vec);

// Initializer-list constructor
eggs::keyed_set<Employee, &Employee::id> c = {
    {3, "Frank", 72'000.0},
    {4, "Grace", 68'000.0},
};
```

---

## Merging two containers

`merge` moves elements from another container into this one. Elements
whose key already exists in the destination are left in the source.

```cpp
eggs::keyed_set<Employee, &Employee::id> primary = {{1, "Alice", 95'000.0}};
eggs::keyed_set<Employee, &Employee::id> extra   = {{2, "Bob", 80'000.0},
                                                    {1, "Duplicate", 0.0}};

primary.merge(extra);
// primary: {1, "Alice", ...}, {2, "Bob", ...}
// extra:   {1, "Duplicate", ...}  — key 1 already existed in primary
```
