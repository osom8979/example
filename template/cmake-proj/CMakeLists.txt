## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

enable_language (C)
enable_language (CXX)

include (cmake/util.cmake)

## CMake settings.
 set (CMAKE_MACOSX_RPATH 1) # CMP0042
#set (CMAKE_VERBOSE_MAKEFILE ON)
#set (BUILD_SHARED_LIBS ON)
 set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC -Wall")
#set (CMAKE_BUILD_TYPE "Debug")
#set (CMAKE_BUILD_TYPE "Release")

#print_os ()
#print_compiler_id ()

## ${PROJECT_SOURCE_DIR}: cmake built-in: source directory.
## ${PROJECT_BINARY_DIR}: cmake built-in: build directory.

set (MAIN_NAME   "main"    CACHE STRING  "Main library name string.")
set (THIRD_NAME  "third"   CACHE STRING  "Third-party library name string.")
set (SAMPLE_NAME "sample"  CACHE STRING  "Sample library name string.")
set (TEST_NAME   "tester"  CACHE STRING  "Test library name string.")

set (VERSION_MAJOR  0)
set (VERSION_MINOR  0)
set (VERSION_PATCH  0)
set (VERSION_TWEAK  0)

configure_file (
    "${PROJECT_SOURCE_DIR}/template/config.h.in"
    "${PROJECT_SOURCE_DIR}/src/${MAIN_NAME}/config.h"
)

include_directories (third)
include_directories (src)

## Excutable
add_subdirectory (third)
add_subdirectory (src)
add_subdirectory (sample)
add_subdirectory (test)

