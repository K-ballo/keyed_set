# **Eggs.KeyedSet**

## Introduction

**Eggs.KeyedSet** is a **C++23** ordered associative container that stores unique
objects indexed by a designated member field.

```cpp
#include <eggs/keyed_set.hpp>

struct Employee { int id; std::string name; double salary; };

eggs::keyed_set<Employee, &Employee::id> roster;

roster.insert({101, "Alice", 95'000.0});
roster.insert({102, "Bob",   80'000.0});

auto it = roster.find(101);   // lookup by int — no Employee constructed
```

Link against `Eggs::KeyedSet` and include `<eggs/keyed_set.hpp>`.

See [docs/](docs/index.html) for design rationale, usage examples, CMake
integration, and the full API reference.

---

> Copyright *Agustín Bergé*, *Fusion Fenix* 2026
>
> Distributed under the Boost Software License, Version 1.0. (See accompanying
> file LICENSE.txt or copy at <http://www.boost.org/LICENSE_1_0.txt>)
