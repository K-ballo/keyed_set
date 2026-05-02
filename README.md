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
    template <typename Value, auto Value::*Key>
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
It has been tested with:

- GCC 14+
- Clang 17+ (with libc++ or libstdc++)
- MSVC 19.38+ (VS 2022 17.8+)

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
| `BUILD_EXAMPLE` | `EGGS_KEYED_SET_BUILD_EXAMPLE` | `ON` / `OFF` | Build the example |
| `ENABLE_INSTALL` | `EGGS_KEYED_SET_ENABLE_INSTALL` | `ON` / `OFF` | Install the library |

### Configuring and building with presets

The project ships a `CMakePresets.json` with named configurations for the
most common workflows. CMake 3.23 or later is required to use presets.

```sh
# List all available presets
cmake --list-presets

# Configure + build + test in one go (system compiler, Debug)
cmake --preset dev
cmake --build --preset dev
ctest --preset dev

# Explicit toolchain variants
cmake --preset dev-gcc    && cmake --build --preset dev-gcc    && ctest --preset dev-gcc
cmake --preset dev-clang  && cmake --build --preset dev-clang  && ctest --preset dev-clang

# Release build
cmake --preset release-gcc && cmake --build --preset release-gcc

# Address + UB sanitizers
cmake --preset asan && cmake --build --preset asan && ctest --preset asan

# Thread sanitizer
cmake --preset tsan && cmake --build --preset tsan && ctest --preset tsan

# Visual Studio 2022 (multi-config)
cmake --preset vs2022
cmake --build --preset vs2022-debug
ctest --preset vs2022
```

Build trees land in `build/<preset-name>/` so all presets coexist without
interfering.

#### Local overrides

Copy the provided `CMakeUserPresets.json` template (it is `.gitignore`d)
to point at a specific compiler installation, set a custom install prefix,
or wire in a vcpkg / Conan toolchain:

```sh
# Example: use a locally built GCC 15
cmake --preset dev-gcc15   # defined only in your CMakeUserPresets.json
```

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
