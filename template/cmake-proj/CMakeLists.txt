cmake_minimum_required (VERSION 2.8.4)

if (UNIX)
    message ("Unix-like OS's.") # including Apple OS X and CygWin.
endif()
if (WIN32)
    message ("Windows OS.") # Prior to 2.8.4 this included CygWin.
endif()
if (APPLE)
    message ("Apple systems.")
endif()

# Compiler & IDE:
# MINGW MSYS CYGWIN BORLAND WATCOM MSVC MSVC_IDE MSVC60 MSVC70 MSVC71 MSVC80 CMAKE_COMPILER_2005 MSVC90 MSVC10

# ${PROJECT_SOURCE_DIR}: cmake built-in: source directory.
# ${PROJECT_BINARY_DIR}: cmake built-in: build directory.

set (MAIN_NAME "main")

project (${MAIN_NAME})

set (${MAIN_NAME}_VERSION_MAJOR 0)
set (${MAIN_NAME}_VERSION_MINOR 0)

configure_file (
    "${PROJECT_SOURCE_DIR}/template/config.h.in"
    "${PROJECT_SOURCE_DIR}/src/${MAIN_NAME}/config.h"
)

add_library (${MAIN_NAME} ${PROJECT_SOURCE_DIR}/src/__test__.cpp)

include_directories (3rd)
include_directories (src)

#set (CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory (sample)
add_subdirectory (test)

