cmake_minimum_required (VERSION 3.28)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("SendHeartBeat")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(ProjectInit)

set(CONFIG_DIR "${CMAKE_BINARY_DIR}/include/")

file (MAKE_DIRECTORY "${SERVER_CONFIG_DIR}")

configure_file(
        "${CMAKE_SOURCE_DIR}/include/config.h.in"
        "${CONFIG_DIR}/config.h"
        @ONLY
)

add_subdirectory(src/Client)
add_subdirectory(src/Server)

add_custom_target(SendHeartBeat ALL DEPENDS Client Server)