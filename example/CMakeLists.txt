# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

add_executable(_eggs_keyed_set_example
    main.cpp)
target_link_libraries(_eggs_keyed_set_example
    PRIVATE
        Eggs::KeyedSet)
target_compile_features(_eggs_keyed_set_example PRIVATE cxx_std_23)
set_target_properties(_eggs_keyed_set_example
    PROPERTIES
        FOLDER "example"
        CXX_EXTENSIONS OFF)
