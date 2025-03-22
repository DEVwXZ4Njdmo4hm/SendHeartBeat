##
#  Copyright 2025 - Capri XXI (qxwzj@hotmail.com)
#  CMake Toolchain File
#  Targeting to: x86_64-linux-gnu
##

set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_ASM_COMPILER as)

set(CMAKE_AR ar)
set(CMAKE_RANLIB ranlib)
set(CMAKE_NM nm)
set(CMAKE_OBJDUMP objdump)
set(CMAKE_OBJCOPY objcopy)
set(CMAKE_STRIP strip)
set(CMAKE_READELF readelf)
set(CMAKE_ADDR2LINE addr2line)
set(CMAKE_SIZE size)

set(CMAKE_LINKER ld)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)