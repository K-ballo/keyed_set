# Eggs.KeyedSet
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.25)
project(Eggs.KeyedSet-FetchContent LANGUAGES CXX)

# -------------------------------------------------------------------------
# EGGS_KEYED_SET_SOURCE_DIR is injected by the CTest fixture so that this
# consumer always tests the local working tree, not a remote tag.
# -------------------------------------------------------------------------
if(NOT DEFINED EGGS_KEYED_SET_SOURCE_DIR)
    message(
        FATAL_ERROR
        "EGGS_KEYED_SET_SOURCE_DIR must be set. "
        "Pass -DEGGS_KEYED_SET_SOURCE_DIR=<path> to cmake."
    )
endif()
file(TO_CMAKE_PATH "${EGGS_KEYED_SET_SOURCE_DIR}" EGGS_KEYED_SET_SOURCE_DIR)

include(FetchContent)
FetchContent_Declare(Eggs.KeyedSet SOURCE_DIR "${EGGS_KEYED_SET_SOURCE_DIR}")
FetchContent_MakeAvailable(Eggs.KeyedSet)

# -------------------------------------------------------------------------
# Verification target — identical intent to the install consumer, but this
# time the target comes from FetchContent rather than an installed config.
# -------------------------------------------------------------------------
add_executable(consumer_verify main.cpp)
target_link_libraries(consumer_verify PRIVATE Eggs::KeyedSet)

# -------------------------------------------------------------------------
# Confirm that the FetchContent target exposes the same properties as the
# installed one (no accidental in-tree-only paths).
# -------------------------------------------------------------------------
foreach(
    _prop
    IN
    ITEMS
        INTERFACE_INCLUDE_DIRECTORIES
        INTERFACE_COMPILE_DEFINITIONS
        INTERFACE_COMPILE_FEATURES
        INTERFACE_LINK_LIBRARIES
)
    get_target_property(_val Eggs::KeyedSet ${_prop})
    message(STATUS "Eggs::KeyedSet (FetchContent) ${_prop}: ${_val}")
endforeach()

enable_testing()
add_test(NAME verify COMMAND consumer_verify)
