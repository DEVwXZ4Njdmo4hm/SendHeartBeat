##
# Copyright (c) 2023, Capri XXI (qxwzj@hotmail.com)
#
# CheckCPUVendor.cmake
# Only Intel x86_64 CPUs are accepted.
#
##

if (NOT (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64" OR
         CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "AMD64")
)
    message(FATAL_ERROR "Unsupported CPU architecture: ${CMAKE_HOST_SYSTEM_PROCESSOR}. Only x86_64 (AMD64) is supported.")
endif ()

# Check if the CPU vendor is Intel
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    execute_process(
        COMMAND powershell -Command "Get-CimInstance -ClassName Win32_Processor | Select-Object -ExpandProperty Manufacturer"
        OUTPUT_VARIABLE CPU_MANUFACTURER
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    string(REGEX REPLACE "Manufacturer[\r\n]+" "" CPU_MANUFACTURER "${CPU_MANUFACTURER}")
    string(STRIP "${CPU_MANUFACTURER}" CPU_MANUFACTURER)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    execute_process(
        COMMAND grep -m1 "^vendor_id" /proc/cpuinfo
        COMMAND cut -d ":" -f2
        OUTPUT_VARIABLE CPU_MANUFACTURER
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    string(STRIP "${CPU_MANUFACTURER}" CPU_MANUFACTURER)
endif ()

if (CPU_MANUFACTURER STREQUAL "GenuineIntel")
    message(STATUS "CPU Manufacturer is Intel.")
else ()
    message(FATAL_ERROR "Unsupported CPU vendor: ${CPU_MANUFACTURER}. Only Intel CPUs are supported.")
endif ()