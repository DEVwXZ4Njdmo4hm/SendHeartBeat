##
# Copyright 2025 - Capri XXI (qxwzj@hotmail.com)
#
# FindLHWM-Wrapper.cmake: Find LHWM-CPP-Wrapper. (LHWM: Libre Hardware Monitor)
#
# Use this module by invoking find_package with the form:
#  find_package(LHWM-Wrapper
#               [REQUIRED]) # Fail with error if lhwm-cpp-wrapper is not found
#
# This module defines the following :prop_tgt:`IMPORTED` target:
# - '' LHWM::LHWM-WRAPPER ''
#
# Results are reported in variables:
# - LHWM-Wrapper_FOUND
# - LHWM-Wrapper_VERSION
# - LHWM-Wrapper_INCLUDE_DIRS
# - LHWM-Wrapper_LIBRARIES
# - LHWM-Wrapper_DEBUG_LIBRARIES
# - LHWM-Wrapper_RELEASE_LIBRARIES
#
##

if (NOT (CMAKE_SYSTEM_NAME STREQUAL "Windows"))
    message(FATAL_ERROR "LHWM wrapper is only supported on Windows.")
endif ()

cmake_policy(PUSH)
cmake_policy(SET CMP0159 NEW)

if (NOT DEFINED LHWM_WRAPPER_DIR OR "${LHWM_WRAPPER_DIR}" STREQUAL "")
    message(FATAL_ERROR "LHWM_WRAPPER_DIR is not set!")
endif ()

# Find for header
find_path (LHWM-Wrapper_INCLUDE_DIR
        NAMES lhwm-cpp-wrapper.h
        PATHS ${LHWM_WRAPPER_DIR}
        PATH_SUFFIXES include
        DOC "LHWM wrapper header"
)

# Find for debug library
find_library(LHWM-Wrapper_DEBUG_LIB
        NAMES lhwm-cpp-wrapper.lib
        PATHS ${LHWM_WRAPPER_DIR}
        PATH_SUFFIXES lib64/Debug
        DOC "LHWM wrapper debug library"
)

# Find for release library
find_library(LHWM-Wrapper_RELEASE_LIB
        NAMES lhwm-cpp-wrapper.lib
        PATHS ${LHWM_WRAPPER_DIR}
        PATH_SUFFIXES lib64/Release
        DOC "LHWM wrapper release library"
)

# Find for debug DLL
find_file (LHWM-Wrapper_DEBUG_DLL
        NAMES lhwm-wrapper.dll
        PATHS ${LHWM_WRAPPER_DIR}
        PATH_SUFFIXES lib64/Debug
        DOC "LHWM wrapper debug DLL"
)

# Find for release DLL
find_file (LHWM-Wrapper_RELEASE_DLL
        NAMES lhwm-wrapper.dll
        PATHS ${LHWM_WRAPPER_DIR}
        PATH_SUFFIXES lib64/Release
        DOC "LHWM wrapper release DLL"
)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args(LHWM-Wrapper
        REQUIRED_VARS
        LHWM-Wrapper_INCLUDE_DIR
        LHWM-Wrapper_DEBUG_LIB
        LHWM-Wrapper_RELEASE_LIB
        LHWM-Wrapper_DEBUG_DLL
        LHWM-Wrapper_RELEASE_DLL
)

if (LHWM-Wrapper_FOUND)
    # detect version.
    file(
            STRINGS ${LHWM-Wrapper_INCLUDE_DIR}/lhwm-cpp-wrapper.h _ver_line
            REGEX   "^#define LHWM_CPP_WRAPPER_VERSION *[0-9]+ *"
            LIMIT_COUNT 1
    )

    string(
            REGEX MATCH "[0-9]+"
            _ver_raw
            "${_ver_line}"
    )

    math(EXPR LHWM_WRAPPER_MAJOR_VERSION    "${_ver_raw} / 100000")
    math(EXPR LHWM_WRAPPER_MINOR_VERSION    "(${_ver_raw} / 100) % 1000")
    math(EXPR LHWM_WRAPPER_SUBMINOR_VERSION "${_ver_raw} % 100")

    string(
            CONCAT
            LHWM_WRAPPER_VERSION
            "${LHWM_WRAPPER_MAJOR_VERSION}."
            "${LHWM_WRAPPER_MINOR_VERSION}."
            "${LHWM_WRAPPER_SUBMINOR_VERSION}"
    )

    unset(_ver_line)
    unset(_ver_raw)

    set(LHWM-Wrapper_VERSION ${LHWM_WRAPPER_VERSION})
    set(LHWM-Wrapper_INCLUDE_DIRS ${LHWM-Wrapper_INCLUDE_DIR})
    set(LHWM-Wrapper_DEBUG_LIBRARIES ${LHWM-Wrapper_DEBUG_LIB})
    set(LHWM-Wrapper_RELEASE_LIBRARIES ${LHWM-Wrapper_RELEASE_LIB})

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(LHWM-Wrapper_LIBRARIES ${LHWM-Wrapper_DEBUG_LIBRARIES})
        set(LHWM-Wrapper_DLL ${LHWM-Wrapper_DEBUG_DLL})
    else ()
        set(LHWM-Wrapper_LIBRARIES ${LHWM-Wrapper_RELEASE_LIBRARIES})
        set(LHWM-Wrapper_DLL ${LHWM-Wrapper_RELEASE_DLL})
    endif ()

    # Create imported target
    add_library(LHWM::LHWM-WRAPPER SHARED IMPORTED)
    set_target_properties(
            LHWM::LHWM-WRAPPER PROPERTIES
            IMPORTED_IMPLIB_DEBUG "${LHWM-Wrapper_DEBUG_LIB}"
            IMPORTED_LOCATION_DEBUG "${LHWM-Wrapper_DEBUG_DLL}"
            IMPORTED_IMPLIB_RELEASE "${LHWM-Wrapper_RELEASE_LIB}"
            IMPORTED_LOCATION_RELEASE "${LHWM-Wrapper_RELEASE_DLL}"
            INTERFACE_INCLUDE_DIRECTORIES "${LHWM-Wrapper_INCLUDE_DIR}"
    )

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set_property(
                TARGET LHWM::LHWM-WRAPPER
                PROPERTY IMPORTED_CONFIGURATIONS DEBUG
        )
    else ()
        set_property(
                TARGET LHWM::LHWM-WRAPPER
                PROPERTY IMPORTED_CONFIGURATIONS RELEASE
        )
    endif ()
endif ()

mark_as_advanced(
        LHWM_WRAPPER_VERSION
        LHWM-Wrapper_INCLUDE_DIR
        LHWM-Wrapper_DEBUG_LIB
        LHWM-Wrapper_RELEASE_LIB
        LHWM-Wrapper_DEBUG_DLL
        LHWM-Wrapper_RELEASE_DLL
)
cmake_policy(POP)