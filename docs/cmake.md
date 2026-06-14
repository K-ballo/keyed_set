<!--
Eggs.KeyedSet

Copyright Agustin K-ballo Berge, Fusion Fenix 2026

Distributed under the Boost Software License, Version 1.0. (See
accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-->

# CMake Integration

## Building

`Eggs.KeyedSet` is header-only. No compilation step is needed unless
you want to build the examples or run the test suite.

```sh
cmake -S . -B build
cmake --build build
```

The project ships `CMakePresets.json` with configurations for common
toolchains. List available presets with:

```sh
cmake --list-presets
```

Configure, build, and test with a single preset name:

```sh
cmake --preset dev-gcc
cmake --build --preset dev-gcc-debug
ctest --preset dev-gcc-debug
```

Available configure presets: `dev-gcc`, `dev-clang`, `dev-clang-libcxx`,
`dev-msvc`, `dev-clang-cl`. Each has matching `debug` and `release` build
and test presets (e.g. `dev-gcc-debug`, `dev-gcc-release`).

### Options

Pass options at configure time with `-D<option>=<value>`.

| Option | Subdirectory variant | Description |
|--------|----------------------|-------------|
| **`BUILD_TESTING`** (default: `ON`) | **`EGGS_KEYED_SET_BUILD_TESTING`** (default: `OFF`) | Build the test suite |
| **`BUILD_EXAMPLES`** (default: `ON`) | **`EGGS_KEYED_SET_BUILD_EXAMPLES`** (default: `OFF`) | Build example programs |
| **`ENABLE_INSTALL`** (default: `ON`) | **`EGGS_KEYED_SET_ENABLE_INSTALL`** (default: `OFF`) | Generate install rules |

The prefixed variants take precedence over the short names when the
project is included via `add_subdirectory`, avoiding collisions with
the parent project's own `BUILD_TESTING` or `BUILD_EXAMPLES` settings.

### Installation

```sh
cmake --install build --prefix /usr/local
```

---

## Consuming the library

### FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
    eggs.keyed_set
    GIT_REPOSITORY https://github.com/eggs-cpp/keyed_set.git
    GIT_TAG        v@VERSION@
)
FetchContent_MakeAvailable(eggs.keyed_set)

target_link_libraries(my_target PRIVATE Eggs::KeyedSet)
```

### add_subdirectory

```cmake
add_subdirectory(vendor/eggs.keyed_set)
target_link_libraries(my_target PRIVATE Eggs::KeyedSet)
```

### find_package (after installation)

```cmake
find_package(Eggs.KeyedSet @VERSION@ REQUIRED)
target_link_libraries(my_target PRIVATE Eggs::KeyedSet)
```

---

## Target

### `Eggs::KeyedSet`

The single library target. Header-only; linking it adds the include
directory containing `<eggs/keyed_set.hpp>` and requires C++23.

```cmake
target_link_libraries(my_target PRIVATE Eggs::KeyedSet)
```
