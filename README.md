# Eggs.KeyedSet

**Eggs.KeyedSet** is a C++23/26 single-header ordered associative container
that stores unique `Value` objects indexed by a designated member field,
satisfying all requirements of the C++26 `AssociativeContainer` named
requirement ([associative.reqmts]).

See the library header at
[`include/eggs/keyed_set.hpp`](include/eggs/keyed_set.hpp).

---

## Synopsis

```cpp
namespace eggs
{
    template <
        typename Value,
        auto     Key,
        typename Compare   = std::less<key-type>,
        typename Allocator = std::allocator<Value>
    >
    class keyed_set;
}
```

`keyed_set<Value, Key>` behaves like a `std::set<Value>` but exposes
`key_type = decltype(v.*Key)` and orders elements by that member.
All heterogeneous lookup operations (`find`, `count`, `contains`,
`lower_bound`, `upper_bound`, `equal_range`, `extract`, `erase`) accept
`key_type` directly, with no `Value` object construction required.

## Example

```cpp
#include <eggs/keyed_set.hpp>

struct Employee { int id; std::string name; double salary; };

eggs::keyed_set<Employee, &Employee::id> roster;

roster.insert({101, "Alice",  95'000.0});
roster.insert({102, "Bob",    80'000.0});

// Heterogeneous lookup by int — no Employee constructed
if (auto it = roster.find(101); it != roster.end())
    std::cout << it->name << '\n';  // Alice

// Iteration is in ascending key order
for (auto const& e : roster)
    std::cout << e.id << ' ' << e.name << '\n';

// from_range constructor
std::vector<Employee> vec{ {1, "Carol", 70'000.0}, {2, "Dave", 80'000.0} };
eggs::keyed_set<Employee, &Employee::id> from_vec(std::from_range, vec);
```

## Requirements

The library requires a C++23-conformant compiler and standard library.
It is continuously tested with:

- GCC 14 and 16
- Clang 18 and 22 (libstdc++ and libc++)
- MSVC 2022 and 2026
- Clang-CL 20 (Windows)

There are no external dependencies.

## Building

The library is header-only. Copy `include/eggs/keyed_set.hpp` into your
project, or use CMake:

```cmake
# As a subdirectory
add_subdirectory(eggs-keyed_set)
target_link_libraries(my_target PRIVATE Eggs::KeyedSet)

# Or via find_package after installation
find_package(Eggs.KeyedSet REQUIRED)
target_link_libraries(my_target PRIVATE Eggs::KeyedSet)
```

### CMake options

| Option (top-level) | Option (subdirectory) | Default | Description |
|---|---|---|---|
| `BUILD_TESTING` | `EGGS_KEYED_SET_BUILD_TESTING` | `ON` / `OFF` | Build the test suite |
| `BUILD_EXAMPLES` | `EGGS_KEYED_SET_BUILD_EXAMPLES` | `ON` / `OFF` | Build the examples |
| `ENABLE_INSTALL` | `EGGS_KEYED_SET_ENABLE_INSTALL` | `ON` / `OFF` | Install the library |

### Configuring and building with presets

The project ships a `CMakePresets.json` with named configurations for the
most common workflows. CMake 3.25 or later is required to use presets.

```sh
# List all available presets
cmake --list-presets

# Configure, build, and test (GCC, Debug)
cmake --preset dev-gcc
cmake --build --preset dev-gcc-debug
ctest --preset dev-gcc-debug

# Other toolchain variants
cmake --preset dev-clang         # Clang with libstdc++
cmake --preset dev-clang-libcxx  # Clang with libc++
cmake --preset dev-msvc          # MSVC x64
cmake --preset dev-clang-cl      # Clang-CL x64
```

Build trees land in `build/<preset-name>/` so all presets coexist without
interfering.

## C++26 AssociativeContainer conformance

`keyed_set` satisfies every expression listed in
[associative.reqmts.general], including:

- All required nested types (`key_type`, `value_type`, `key_compare`,
  `value_compare`, `allocator_type`, `node_type`, `insert_return_type`, …)
- Constant bidirectional iterators (`iterator == const_iterator`)
- All constructor forms: default, comparator, iterator-range,
  `from_range`, initializer-list, copy, move, allocator-extended variants
- `operator=(initializer_list)`
- `emplace`, `emplace_hint`
- `insert` (lvalue, rvalue, hint, iterator-range, `insert_range`, initializer-list,
  node-handle, hint + node-handle)
- `extract` (by iterator, by key, transparent)
- `erase` (by key, transparent, by iterator, by range)
- `clear`, `merge`
- `erase_if(keyed_set&, Predicate)` (free function)
- `key_comp`, `value_comp`
- `find`, `count`, `contains`, `lower_bound`, `upper_bound`, `equal_range`
  — all with both exact and transparent (`K const&`) overloads
- `operator==` and `operator<=>` (synthesises all six relational operators)
- Member and non-member `swap`

## License

Distributed under the Boost Software License, Version 1.0.
(See accompanying file [LICENSE.txt](LICENSE.txt) or copy at
<http://www.boost.org/LICENSE_1_0.txt>)

Copyright Agustin K-ballo Berge, Fusion Fenix 2026
