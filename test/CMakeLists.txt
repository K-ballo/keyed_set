# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# ── Vendored Catch2 v2 single-header ────────────────────────────────────────

add_library(_catch2 INTERFACE)
target_include_directories(_catch2 SYSTEM INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/catch")
target_compile_definitions(_catch2 INTERFACE
    CATCH_CONFIG_FAST_COMPILE)   # cuts per-TU overhead in v2

# ── Shared test main (compiled once, linked into every test executable) ──────

add_library(_eggs_keyed_set_test_main OBJECT
    main.cpp)
target_link_libraries(_eggs_keyed_set_test_main
    PUBLIC
        Eggs::KeyedSet
        _catch2)
set_target_properties(_eggs_keyed_set_test_main
    PROPERTIES
        FOLDER "test"
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        CXX_SCAN_FOR_MODULES OFF)

# ── One executable + one CTest per test source ───────────────────────────────

set(_test_sources
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

foreach(_src IN LISTS _test_sources)
    # Derive a target name from the filename: e.g. cnstr.default.cpp -> cnstr.default
    get_filename_component(_name "${_src}" NAME_WLE)
    set(_target "_eggs_keyed_set_test.${_name}")

    add_executable(${_target} "${_src}")
    target_link_libraries(${_target}
        PRIVATE _eggs_keyed_set_test_main)
    set_target_properties(${_target}
        PROPERTIES
            FOLDER "test"
            CXX_STANDARD 23
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            CXX_SCAN_FOR_MODULES OFF)

    add_test(
        NAME "keyed_set.${_name}"
        COMMAND ${_target})
endforeach()
