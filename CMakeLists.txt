# Eggs.KeyedSet
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.25...4.2.3)

project(
    Eggs.KeyedSet
    VERSION 1.0.0
    DESCRIPTION "A C++23 keyed-set container"
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)

# Options (two spellings: short when top-level, prefixed when a subdirectory)
if(PROJECT_IS_TOP_LEVEL)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

    option(BUILD_DOCS "Build the documentation" ON)
    option(BUILD_EXAMPLES "Build the examples" ON)
    include(CTest) # option(BUILD_TESTING ...)
    option(ENABLE_INSTALL "Generate install rules" ON)
    set(SYSTEM OFF)
else()
    option(EGGS_KEYED_SET_BUILD_DOCS "Build the documentation" OFF)
    option(EGGS_KEYED_SET_BUILD_EXAMPLES "Build the examples" OFF)
    option(EGGS_KEYED_SET_BUILD_TESTING "Build the testing tree" OFF)
    option(EGGS_KEYED_SET_ENABLE_INSTALL "Generate install rules" OFF)

    # Map prefixed options back to the canonical names used below
    set(BUILD_DOCS ${EGGS_KEYED_SET_BUILD_DOCS})
    set(BUILD_EXAMPLES ${EGGS_KEYED_SET_BUILD_EXAMPLES})
    set(BUILD_TESTING ${EGGS_KEYED_SET_BUILD_TESTING})
    set(ENABLE_INSTALL ${EGGS_KEYED_SET_ENABLE_INSTALL})
    set(SYSTEM ON)
endif()

#
# Library target — header-only INTERFACE library.
# Internal name _eggs_keyed_set; downstream CMake code uses the alias Eggs::KeyedSet.
#
add_library(_eggs_keyed_set INTERFACE)
target_sources(
    _eggs_keyed_set
    INTERFACE
        FILE_SET HEADERS BASE_DIRS include FILES include/eggs/keyed_set.hpp
)
set_target_properties(
    _eggs_keyed_set
    PROPERTIES
        EXPORT_NAME KeyedSet
        SYSTEM ${SYSTEM}
        VERIFY_INTERFACE_HEADER_SETS ${BUILD_TESTING}
)
target_compile_features(_eggs_keyed_set INTERFACE cxx_std_23)

add_library(Eggs::KeyedSet ALIAS _eggs_keyed_set)

#
# Documentation
#
if(BUILD_DOCS)
    add_subdirectory(docs)
endif()

#
# Examples
#
if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

#
# Test suite
#
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()

#
# Install rules
#
if(ENABLE_INSTALL)
    include(CMakePackageConfigHelpers)

    install(
        TARGETS _eggs_keyed_set
        EXPORT EggsKeyedSetTargets
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(
        EXPORT EggsKeyedSetTargets
        NAMESPACE Eggs::
        FILE eggs.keyedset-targets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/eggs.keyedset
    )

    configure_package_config_file(
        cmake/eggs.keyedset-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/eggs.keyedset-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/eggs.keyedset
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/eggs.keyedset-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/eggs.keyedset-config.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/eggs.keyedset-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/eggs.keyedset
    )

    set(CPACK_PACKAGE_NAME "Eggs.KeyedSet")
    set(CPACK_PACKAGE_VENDOR "Agustin K-ballo Berge, Fusion Fenix")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
    if(WIN32)
        set(CPACK_GENERATOR ZIP)
    else()
        set(CPACK_GENERATOR TGZ)
    endif()
    include(CPack)
endif()
