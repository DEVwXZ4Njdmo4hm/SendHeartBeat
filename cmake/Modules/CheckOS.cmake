##
# Copyright (c) 2023, Capri XXI (qxwzj@hotmail.com)
#
# CheckOS.cmake
# Only Windows and Linux are accepted.
#
##


if (CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Build on supported OS: ${CMAKE_SYSTEM_NAME}")
else()
    message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif()