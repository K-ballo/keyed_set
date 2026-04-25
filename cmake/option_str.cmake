# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# option_str(<variable> <help_text> <value>)
#
# Provides a cache variable of type STRING, analogous to option() for BOOL.
macro(option_str variable help_text value)
    set(${variable} "${value}" CACHE STRING "${help_text}")
endmacro()
