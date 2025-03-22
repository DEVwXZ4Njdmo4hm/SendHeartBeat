##
# Copyright 2025 - Capri XXI (qxwzj@hotmail.com)
#
# FindASIO.cmake: Find ASIO headers.
#
# Use this module by invoking find_package with the form:
#  find_package(ASIO
#               [REQUIRED]) # Fail with error if asio is not found
#
# This module defines the following :prop_tgt:`IMPORTED` target:
# - '' BOOST::ASIO''
#
# Results are reported in variables:
# - ASIO_FOUND
# - ASIO_VERSION
# - ASIO_INCLUDE_DIR
#
##

cmake_policy(PUSH)
cmake_policy(SET CMP0159 NEW)

if (NOT DEFINED ASIO_DIR OR "${ASIO_DIR}" STREQUAL "")
    message(FATAL_ERROR "ASIO_DIR is not set!")
endif ()

message(STATUS "Searching for ASIO in ${ASIO_DIR}")

if (EXISTS ${ASIO_DIR} AND
        EXISTS ${ASIO_DIR}/lib64/pkgconfig/asio.pc.in
)
    if (LINUX)
        configure_file(
                ${ASIO_DIR}/lib64/pkgconfig/asio.pc.in
                ${PROJECT_BINARY_DIR}/asio.pc
                @ONLY
        )

        set(ASIO_PKGCONFIG_PATH "${PROJECT_BINARY_DIR}")
        set(ENV{PKG_CONFIG_PATH} "${ASIO_PKGCONFIG_PATH}:$ENV{PKG_CONFIG_PATH}")

        find_package(PkgConfig QUIET REQUIRED)
        if (PKG_CONFIG_FOUND)
            pkg_check_modules(PC_ASIO REQUIRED QUIET asio)
        else ()
            message(FATAL_ERROR "Can not found pkgconfig!")
        endif ()

        if (NOT PC_ASIO_FOUND)
            message(FATAL_ERROR "PkgConfig can not found ASIO.")
        endif ()

        set(FOUND_ASIO_DIR "${PC_ASIO_PREFIX}")
    else ()
        # On Windows, bypass pkg-config and directly use the predefined ASIO_DIR 
        # to avoid path resolution problems
        set(FOUND_ASIO_DIR "${ASIO_DIR}")
    endif()

    find_path(
            ASIO_INCLUDE_DIR
            NAMES   asio.hpp
            HINTS   ${FOUND_ASIO_DIR}/include
            NO_DEFAULT_PATH
    )
    mark_as_advanced(ASIO_INCLUDE_DIR)

    if (ASIO_INCLUDE_DIR)
        # Extract version information directly from header files 
        # instead of relying on asio.pc metadata.
        
        file(
                STRINGS ${ASIO_INCLUDE_DIR}/asio/version.hpp _ver_line
                REGEX   "^#define ASIO_VERSION *[0-9]+ *"
                LIMIT_COUNT 1
        )

        string(
                REGEX MATCH "[0-9]+"
                _ver_raw
                "${_ver_line}"
        )

        math(EXPR ASIO_MAJOR_VERSION    "${_ver_raw} / 100000")
        math(EXPR ASIO_MINOR_VERSION    "(${_ver_raw} / 100) % 1000")
        math(EXPR ASIO_SUBMINOR_VERSION "${_ver_raw} % 100")

        string(
                CONCAT
                ASIO_VERSION
                "${ASIO_MAJOR_VERSION}."
                "${ASIO_MINOR_VERSION}."
                "${ASIO_SUBMINOR_VERSION}"
        )

        unset(_ver_line)
        unset(_ver_raw)
    endif ()

    set(ASIO_FOUND ON)
    add_library(BOOST::ASIO INTERFACE IMPORTED)
    set_target_properties(
            BOOST::ASIO PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES   "${ASIO_INCLUDE_DIR}"
    )
endif ()

cmake_policy(POP)