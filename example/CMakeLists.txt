# Eggs.KeyedSet
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

add_executable(_eggs_keyed_set_example main.cpp)
target_link_libraries(_eggs_keyed_set_example PRIVATE Eggs::KeyedSet)
set_target_properties(_eggs_keyed_set_example PROPERTIES FOLDER "example")

if(BUILD_TESTING)
    add_test(NAME eggs.keyed_set.example COMMAND _eggs_keyed_set_example)
    set_tests_properties(eggs.keyed_set.example PROPERTIES LABELS "example")
endif()
