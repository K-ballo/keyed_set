# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# ── Vendored Catch2 v2 single-header ────────────────────────────────────────

add_library(_catch2 INTERFACE)
target_include_directories(_catch2 INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/catch")
target_compile_definitions(_catch2 INTERFACE
    CATCH_CONFIG_FAST_COMPILE)   # cuts per-TU overhead in v2

# ── Test executable ──────────────────────────────────────────────────────────

set(_test_sources
    main.cpp
    types.cpp
    cnstr.default.cpp
    cnstr.compare.cpp
    cnstr.range.cpp
    cnstr.from_range.cpp
    cnstr.init_list.cpp
    cnstr.copy.cpp
    cnstr.move.cpp
    assign.copy.cpp
    assign.move.cpp
    assign.init_list.cpp
    modif.emplace.cpp
    modif.insert.cpp
    modif.insert_range.cpp
    modif.extract.cpp
    modif.erase.cpp
    modif.clear.cpp
    modif.merge.cpp
    lookup.find.cpp
    lookup.count.cpp
    lookup.contains.cpp
    lookup.bounds.cpp
    lookup.equal_range.cpp
    observers.cpp
    swap.cpp
    comparison.cpp
)

add_executable(_eggs_keyed_set_test ${_test_sources})
target_link_libraries(_eggs_keyed_set_test
    PRIVATE
        Eggs::KeyedSet
        _catch2)
target_compile_features(_eggs_keyed_set_test PRIVATE cxx_std_23)
set_target_properties(_eggs_keyed_set_test
    PROPERTIES
        FOLDER "test"
        CXX_EXTENSIONS OFF)

add_test(
    NAME keyed_set
    COMMAND _eggs_keyed_set_test)
