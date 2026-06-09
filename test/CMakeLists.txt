# Eggs.KeyedSet
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# ── Verify interface header sets ────────────────────────────────────────────

add_test(
    NAME eggs.keyed_set.verify_headers
    COMMAND
        ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target
        _eggs_keyed_set_verify_interface_header_sets --config $<CONFIG>
)
set_tests_properties(
    eggs.keyed_set.verify_headers
    PROPERTIES LABELS "header" RESOURCE_LOCK cmake_build
)

# ── Verify definitions in headers ──────────────────────────────────────────

include(AddMultipleDefinitionsTest)

add_multiple_definitions_test(eggs.keyed_set.verify_definitions Eggs::KeyedSet)
set_target_properties(
    eggs.keyed_set.verify_definitions
    PROPERTIES FOLDER "header"
)
set_tests_properties(
    eggs.keyed_set.verify_definitions
    PROPERTIES LABELS "header" RESOURCE_LOCK cmake_build
)

# ── Vendored Catch2 v2 single-header ────────────────────────────────────────

add_library(_catch2 OBJECT catch2/catch.cpp)
target_link_libraries(_catch2 PUBLIC Eggs::KeyedSet)
target_include_directories(
    _catch2
    SYSTEM
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/catch2"
)
target_compile_definitions(_catch2 INTERFACE CATCH_CONFIG_FAST_COMPILE)
set_target_properties(_catch2 PROPERTIES FOLDER "test")

# ── Tests ────────────────────────────────────────────────────────────────────

# Helper — creates a test executable and registers it with CTest.
function(add_lib_test name)
    add_executable(_eggs_keyed_set_test.${name} ${name}.cpp)
    target_link_libraries(_eggs_keyed_set_test.${name} PRIVATE _catch2)
    set_target_properties(_eggs_keyed_set_test.${name} PROPERTIES FOLDER "test")

    add_test(NAME eggs.keyed_set.${name} COMMAND _eggs_keyed_set_test.${name})
    set_tests_properties(eggs.keyed_set.${name} PROPERTIES LABELS "unit")
endfunction()

add_lib_test(types)
add_lib_test(cnstr.default)
add_lib_test(cnstr.compare)
add_lib_test(cnstr.range)
add_lib_test(cnstr.from_range)
add_lib_test(cnstr.init_list)
add_lib_test(cnstr.copy)
add_lib_test(cnstr.move)
add_lib_test(assign.copy)
add_lib_test(assign.move)
add_lib_test(assign.init_list)
add_lib_test(modif.emplace)
add_lib_test(modif.erase_if)
add_lib_test(modif.insert)
add_lib_test(modif.insert_range)
add_lib_test(modif.extract)
add_lib_test(modif.erase)
add_lib_test(modif.clear)
add_lib_test(modif.merge)
add_lib_test(lookup.find)
add_lib_test(lookup.count)
add_lib_test(lookup.contains)
add_lib_test(lookup.bounds)
add_lib_test(lookup.equal_range)
add_lib_test(observers)
add_lib_test(compare)
add_lib_test(allocator)
add_lib_test(swap)
add_lib_test(comparison)
