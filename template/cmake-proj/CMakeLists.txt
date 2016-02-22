## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

## CMake settings.
set (CMAKE_VERBOSE_MAKEFILE  ON)
set (CMAKE_MACOSX_RPATH       1) # CMP0042

enable_language (C)
enable_language (CXX)

include (cmake/util.cmake)

echo_os ()
echo_compiler_id ()

# ${PROJECT_SOURCE_DIR}: cmake built-in: source directory.
# ${PROJECT_BINARY_DIR}: cmake built-in: build directory.

## ------------------------------
## Enter the name of the library.
set (MAIN_NAME "main")
## ------------------------------

project (${MAIN_NAME} C CXX)

set (${MAIN_NAME}_VERSION_MAJOR 0)
set (${MAIN_NAME}_VERSION_MINOR 0)
set (${MAIN_NAME}_VERSION_PATCH 0)
set (${MAIN_NAME}_VERSION_TWEAK 0)

set (MAIN_STATIC ${MAIN_NAME}_static)
set (MAIN_SHARED ${MAIN_NAME})

configure_file (
    "${PROJECT_SOURCE_DIR}/template/config.h.in"
    "${PROJECT_SOURCE_DIR}/src/${MAIN_NAME}/config.h"
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set (MAIN_LINK -Wl,-force_load ${MAIN_STATIC})
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set (MAIN_LINK -Wl,--whole-archive ${MAIN_STATIC} -Wl,--no-whole-archive)
endif ()

add_library (${MAIN_STATIC} STATIC ${PROJECT_SOURCE_DIR}/src/__test__.cpp)
add_library (${MAIN_SHARED} SHARED)
add_dependencies (${MAIN_SHARED} ${MAIN_STATIC})
#set_target_properties (${MAIN_STATIC} PROPERTIES OUTPUT_NAME ${MAIN_NAME})
target_link_libraries (${MAIN_SHARED} ${MAIN_LINK})

include_directories (3rd)
include_directories (src)

add_subdirectory (sample)
add_subdirectory (test)

