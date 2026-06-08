# Eggs.KeyedSet
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.25)
project(Eggs.KeyedSet-FindPackage LANGUAGES CXX)

# -------------------------------------------------------------------------
# Find the installed package exactly as an end-user would
# -------------------------------------------------------------------------
find_package(Eggs.KeyedSet REQUIRED)

# -------------------------------------------------------------------------
# Verification target
# Compiles a minimal translation unit that exercises the interface headers
# and verifies INTERFACE_COMPILE_DEFINITIONS / INTERFACE_INCLUDE_DIRECTORIES
# were exported correctly.
# -------------------------------------------------------------------------
add_executable(consumer_verify main.cpp)
target_link_libraries(consumer_verify PRIVATE Eggs::KeyedSet)

# -------------------------------------------------------------------------
# Print target properties at configure time so ctest -V shows them
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
    message(STATUS "Eggs::KeyedSet ${_prop}: ${_val}")
endforeach()

enable_testing()
add_test(NAME verify COMMAND consumer_verify)
