## CMake main build script.

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

#echo_os ()
#echo_compiler_id ()

## ${PROJECT_SOURCE_DIR}: cmake built-in: source directory.
## ${PROJECT_BINARY_DIR}: cmake built-in: build directory.

## ------------------------------
## Enter the name of the library.
set (MAIN_NAME "main")
## ------------------------------

project (${MAIN_NAME} C CXX)

set (${MAIN_NAME}_VERSION_MAJOR  0)
set (${MAIN_NAME}_VERSION_MINOR  0)
set (${MAIN_NAME}_VERSION_PATCH  0)
set (${MAIN_NAME}_VERSION_TWEAK  0)

configure_file (
    "${PROJECT_SOURCE_DIR}/template/config.h.in"
    "${PROJECT_SOURCE_DIR}/src/${MAIN_NAME}/config.h"
)

include_directories (third)
include_directories (src)

add_library (${MAIN_NAME} ${PROJECT_SOURCE_DIR}/src/__test__.cpp)

## Excutable
add_subdirectory (sample)
add_subdirectory (test)

