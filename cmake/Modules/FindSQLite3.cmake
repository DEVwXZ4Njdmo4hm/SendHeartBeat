##
# Copyright 2025 - Capri XXI (qxwzj@hotmail.com)
#
# FindSQLite3.cmake: Find SQLite3 headers and libraries
#
# Use this module by invoking find_package with the form:
#  find_package(SQLite3
#               [REQUIRED]) # Fail with error if sqlite3 is not found
#
# This module defines the following :prop_tgt:`IMPORTED` target:
# - '' SQLite::SQLite3''
#
# Results are reported in variables:
# - SQLite3_FOUND
# - SQLite3_VERSION
# - SQLite3_INCLUDE_DIR
# - SQLite3_LIBRARIES
# On Windows, an additional variable are defined:
# - SQLite3_DLL
#
##

cmake_policy(PUSH)
cmake_policy(SET CMP0159 NEW)

if (NOT DEFINED SQLITE_META_DIR OR "${SQLITE_META_DIR}" STREQUAL "")
    message(FATAL_ERROR "SQLITE_META_DIR is not set!")
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set (SQLITE3_DIR "${SQLITE_META_DIR}/windows")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set (SQLITE3_DIR "${SQLITE_META_DIR}/linux")
endif ()

if (EXISTS ${SQLITE3_DIR} AND
        EXISTS ${SQLITE3_DIR}/bin AND
        EXISTS ${SQLITE3_DIR}/lib64 AND
        EXISTS ${SQLITE3_DIR}/include
)
    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        find_path(
                SQLite3_INCLUDE_DIR
                NAMES   sqlite3.h
                PATHS   ${SQLITE3_DIR}
                PATH_SUFFIXES include
                NO_DEFAULT_PATH
        )

        find_library(
                SQLite3_LIBRARY
                NAMES   sqlite3.lib
                PATHS   ${SQLITE3_DIR}
                PATH_SUFFIXES lib64
                NO_DEFAULT_PATH
        )

        find_file(
                SQLite3_DYLIB
                NAMES   sqlite3.dll
                PATHS   ${SQLITE3_DIR}
                PATH_SUFFIXES lib64
                NO_DEFAULT_PATH
        )
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        # On Linux, use pkg-config to find SQLite3
        if (NOT EXISTS ${SQLITE3_DIR}/lib64/pkgconfig/sqlite3.pc.in)
            message(FATAL_ERROR "SQLite3 pkgconfig file not found!")
        endif ()

        configure_file(
                ${SQLITE3_DIR}/lib64/pkgconfig/sqlite3.pc.in
                ${PROJECT_BINARY_DIR}/sqlite3.pc
                @ONLY
        )

        set(SQLITE3_PKGCONFIG_PATH "${PROJECT_BINARY_DIR}")
        set(ENV{PKG_CONFIG_PATH} "${SQLITE3_PKGCONFIG_PATH}:$ENV{PKG_CONFIG_PATH}")

        find_package(PkgConfig QUIET REQUIRED)
        if (PKG_CONFIG_FOUND)
            pkg_check_modules(PC_SQLite3 QUIET sqlite3)
        else ()
            message(FATAL_ERROR "Can not found pkgconfig!")
        endif ()

        if (NOT PC_SQLite3_FOUND)
            message(FATAL_ERROR "PkgConfig can not found sqlite3.")
        endif ()

        find_path(
                SQLite3_INCLUDE_DIR
                NAMES   sqlite3.h
                HINTS   ${PC_SQLite3_INCLUDE_DIRS}
                NO_DEFAULT_PATH
        )
        mark_as_advanced(SQLite3_INCLUDE_DIR)

        find_library(
                SQLite3_LIBRARY
                NAMES   sqlite3 sqlite
                HINTS   ${PC_SQLite3_LIBRARY_DIRS}
                NO_DEFAULT_PATH
        )
        mark_as_advanced(SQLite3_LIBRARY)
    endif ()

    if (SQLite3_INCLUDE_DIR)
        file(
                STRINGS ${SQLite3_INCLUDE_DIR}/sqlite3.h _ver_line
                REGEX "^#define SQLITE_VERSION  *\"[0-9]+\\.[0-9]+\\.[0-9]+\""
                LIMIT_COUNT 1
        )
        string(
                REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+"
                SQLite3_VERSION
                "${_ver_line}"
        )
        unset(_ver_line)
    endif ()

    set(SQLite3_FOUND ON)
    set(SQLite3_INCLUDE_DIRS ${SQLite3_INCLUDE_DIR})
    set(SQLite3_LIBRARIES ${SQLite3_LIBRARY})

    add_library(SQLite::SQLite3 SHARED IMPORTED)

    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

        set(SQLITE3_DLL "${SQLITE3_DYLIB}")

        set_target_properties(
                SQLite::SQLite3 PROPERTIES
                IMPORTED_IMPLIB                     "${SQLite3_LIBRARY}"
                IMPORTED_LOCATION                   "${SQLite3_DLL}"
                INTERFACE_INCLUDE_DIRECTORIES       "${SQLite3_INCLUDE_DIR}"
        )

        mark_as_advanced(
                SQLite3_INCLUDE_DIR
                SQLite3_LIBRARY
                SQLITE3_DYLIB
        )
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set_target_properties(
                SQLite::SQLite3 PROPERTIES
                IMPORTED_LOCATION                   "${SQLite3_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES       "${SQLite3_INCLUDE_DIR}"
        )
    endif ()

else ()
    message(FATAL_ERROR "SQLite3 not found!")
endif ()

cmake_policy(POP)