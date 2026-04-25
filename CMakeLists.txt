# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.24...3.31)

project(Eggs.KeyedSet CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Options (two spellings: short when top-level, prefixed when a subdirectory)
if(PROJECT_IS_TOP_LEVEL)
    include(CTest)  # option(BUILD_TESTING ...)
    option(BUILD_EXAMPLE "Build the example" ON)
    option(ENABLE_INSTALL "Enable installing the library" ON)
else()
    option(EGGS_KEYED_SET_BUILD_TESTING "Build the testing tree" OFF)
    option(EGGS_KEYED_SET_BUILD_EXAMPLE "Build the example" OFF)
    option(EGGS_KEYED_SET_ENABLE_INSTALL "Enable installing the library" OFF)

    # Map prefixed options back to the canonical names used below
    if(EGGS_KEYED_SET_BUILD_TESTING)
        set(BUILD_TESTING ON)
    endif()
    if(EGGS_KEYED_SET_BUILD_EXAMPLE)
        set(BUILD_EXAMPLE ON)
    endif()
    if(EGGS_KEYED_SET_ENABLE_INSTALL)
        set(ENABLE_INSTALL ON)
    endif()
endif()

# Library target (header-only INTERFACE library)
add_library(_eggs_keyed_set INTERFACE)
target_include_directories(_eggs_keyed_set INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(_eggs_keyed_set INTERFACE cxx_std_23)
target_sources(_eggs_keyed_set INTERFACE
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/eggs/keyed_set.hpp)
set_target_properties(_eggs_keyed_set
    PROPERTIES
        EXPORT_NAME Eggs::KeyedSet
        VERIFY_INTERFACE_HEADER_SETS ${BUILD_TESTING})

add_library(Eggs::KeyedSet ALIAS _eggs_keyed_set)

# Example
if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

# Test
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()

# Install
if(ENABLE_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(TARGETS _eggs_keyed_set EXPORT _targets
        FILE_SET HEADERS)
    install(EXPORT _targets
        NAMESPACE Eggs::
        FILE EggsKeyedSetTargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Eggs.KeyedSet)

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EggsKeyedSetConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/EggsKeyedSetConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Eggs.KeyedSet)

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/EggsKeyedSetConfigVersion.cmake
        VERSION 1.0.0
        COMPATIBILITY SameMajorVersion)

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/EggsKeyedSetConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/EggsKeyedSetConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Eggs.KeyedSet)
endif()
