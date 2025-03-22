##
#
# Copyright 2025, Capri XXI (qxwzj@hotmail.com)
#
# ApplyProjectConfig.cmake
# This script applies the project configuration from @CMAKE_SOURCE_DIR@/project-config.txt
#
##

file (STRINGS "${CMAKE_SOURCE_DIR}/project-config.txt" config_lines)
foreach(line ${config_lines})
    if(line MATCHES "^[ \t]*([^=]+)[ \t]*=[ \t]*(.*)$")

        # Clear possible leading and trailing whitespace from variable name and value
        string(STRIP "${CMAKE_MATCH_1}" var_name)
        string(STRIP "${CMAKE_MATCH_2}" var_value)

        # Check and remove any quote
        if(var_value MATCHES "^\"(.*)\"$")
            set(var_value "${CMAKE_MATCH_1}")
        elseif(var_value MATCHES "^'(.*)'$")
            set(var_value "${CMAKE_MATCH_1}")
        endif()

        # Process variable references
        string(CONFIGURE "${var_value}" var_value_expanded)

        # Remove leading and trailing whitespace from expanded variable value
        string(STRIP "${var_value_expanded}" var_value_expanded)

        # Set the variable in CMake
        set(${var_name} "${var_value_expanded}")
        message(STATUS "Config: ${var_name} = ${var_value_expanded}")
    endif()
endforeach(line ${config_lines})