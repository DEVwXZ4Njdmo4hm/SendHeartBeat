##
#  Copyright 2025 - Capri XXI (qxwzj@hotmail.com)
#  CMake Toolchain File
#  Targeting to: x86_64-linux-gnu
##
set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_ASM_COMPILER clang)

set(CMAKE_AR llvm-ar)
set(CMAKE_RANLIB llvm-ranlib)
set(CMAKE_NM llvm-nm)
set(CMAKE_OBJDUMP llvm-objdump)
set(CMAKE_OBJCOPY llvm-objcopy)
set(CMAKE_STRIP llvm-strip)
set(CMAKE_READELF llvm-readelf)
set(CMAKE_ADDR2LINE llvm-addr2line)
set(CMAKE_SIZE llvm-size)

set(CMAKE_LINKER lld)
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-fuse-ld=lld")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)