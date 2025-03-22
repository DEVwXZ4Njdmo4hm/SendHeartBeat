##
# Copyright 2025 - Capri XXI (qxwzj@hotmail.com)
#
# FindPFR.cmake: Find PFR headers.
#
# Use this module by invoking find_package with the form:
#  find_package(PFR
#               [REQUIRED]) # Fail with error if pfr is not found
#
# This module defines the following :prop_tgt:`IMPORTED` target:
# - '' BOOST::PFR''
#
# Results are reported in variables:
# - PFR_FOUND
# - PFR_VERSION
# - PFR_INCLUDE_DIR
#
##

cmake_policy(PUSH)
cmake_policy(SET CMP0159 NEW)

if (NOT DEFINED PFR_DIR OR "${PFR_DIR}" STREQUAL "")
    message(FATAL_ERROR "PFR_DIR is not set!")
endif ()

message(STATUS "Searching for PFR in ${PFR_DIR}")

if (EXISTS ${PFR_DIR} AND
        EXISTS ${PFR_DIR}/lib64/pkgconfig/pfr.pc.in
)
    if (LINUX)
        configure_file(
                ${PFR_DIR}/lib64/pkgconfig/pfr.pc.in
                ${PROJECT_BINARY_DIR}/pfr.pc
                @ONLY
        )

        set(PFR_PKGCONFIG_PATH "${PROJECT_BINARY_DIR}")
        set(ENV{PKG_CONFIG_PATH} "${PFR_PKGCONFIG_PATH}:$ENV{PKG_CONFIG_PATH}")

        find_package(PkgConfig QUIET REQUIRED)
        if (PKG_CONFIG_FOUND)
            pkg_check_modules(PC_PFR REQUIRED QUIET pfr)
        else ()
            message(FATAL_ERROR "Can not found pkgconfig!")
        endif ()

        if (NOT PC_PFR_FOUND)
            message(FATAL_ERROR "PkgConfig can not found PFR.")
        endif ()

        set(FOUND_PFR_DIR "${PC_PFR_PREFIX}")
    else ()
        # On Windows, bypass pkg-config and directly use the predefined PFR_DIR
        # to avoid path resolution problems
        set(FOUND_PFR_DIR "${PFR_DIR}")
    endif()

    find_path(
            PFR_INCLUDE_DIR
            NAMES   pfr.hpp
            HINTS   ${FOUND_PFR_DIR}/include
            NO_DEFAULT_PATH
    )
    mark_as_advanced(PFR_INCLUDE_DIR)

    if (PFR_INCLUDE_DIR)
        # Extract version information directly from header files
        # instead of relying on pfr.pc metadata.

        file(
                STRINGS ${PFR_INCLUDE_DIR}/pfr/version.hpp _ver_line
                REGEX   "^#define PFR_VERSION *[0-9]+ *"
                LIMIT_COUNT 1
        )

        string(
                REGEX MATCH "[0-9]+"
                _ver_raw
                "${_ver_line}"
        )

        math(EXPR PFR_MAJOR_VERSION    "${_ver_raw} / 100000")
        math(EXPR PFR_MINOR_VERSION    "(${_ver_raw} / 100) % 1000")
        math(EXPR PFR_SUBMINOR_VERSION "${_ver_raw} % 100")

        string(
                CONCAT
                PFR_VERSION
                "${PFR_MAJOR_VERSION}."
                "${PFR_MINOR_VERSION}."
                "${PFR_SUBMINOR_VERSION}"
        )

        unset(_ver_line)
        unset(_ver_raw)
    endif ()

    set(PFR_FOUND ON)
    add_library(BOOST::PFR INTERFACE IMPORTED)
    set_target_properties(
            BOOST::PFR PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES   "${PFR_INCLUDE_DIR}"
    )
endif ()

cmake_policy(POP)