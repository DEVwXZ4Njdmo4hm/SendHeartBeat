##
# Copyright (c) 2023, Capri XXI (qxwzj@hotmail.com)
#
# SetupLLVM.cmake
# Setup LLVM for project, only Linux is supported.
#
##

if (NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME} to use LLVM.")
endif()

find_package(LLVM CONFIG REQUIRED)
set(USE_LLVM ON)
message(STATUS "Found LLVM version ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM include directories: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM libraries: ${LLVM_LIBRARIES}")
message(STATUS "LLVM tools: ${LLVM_TOOLS_BINARY_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi -lunwind")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld")

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

function(configure_for_llvm target)
    target_include_directories(${target} PRIVATE ${LLVM_INCLUDE_DIRS})
    target_link_libraries(${target} PRIVATE ${LLVM_LIBRARIES} c++ c++abi unwind)
endfunction()