cmake_minimum_required(VERSION 3.28)

add_executable(Server)

target_sources(Server
        PRIVATE
        Server.cpp
)

set_target_properties(Server PROPERTIES CXX_STANDARD 23)
target_include_directories(Server PRIVATE "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(Server PRIVATE
        BOOST::PFR
        BOOST::ASIO
        SQLite::SQLite3
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_options(Server PRIVATE "/D_WIN32_WINNT=0x0601" "/utf-8")
endif()