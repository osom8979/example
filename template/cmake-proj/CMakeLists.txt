cmake_minimum_required (VERSION 2.8.8)

if (UNIX)
    message ("Unix-like OS's.") # including Apple OS X and CygWin.
endif ()
if (WIN32)
    message ("Windows OS.") # Prior to 2.8.4 this included CygWin.
endif ()
if (APPLE)
    message ("Apple systems.")
endif ()

# Compiler & IDE:
# MINGW MSYS CYGWIN BORLAND WATCOM MSVC MSVC_IDE MSVC60 MSVC70 MSVC71 MSVC80 CMAKE_COMPILER_2005 MSVC90 MSVC10

# ${PROJECT_SOURCE_DIR}: cmake built-in: source directory.
# ${PROJECT_BINARY_DIR}: cmake built-in: build directory.

set (MAIN_NAME "main")
set (MAIN_NAME_STATIC ${MAIN_NAME}_static)
set (MAIN_NAME_SHARED ${MAIN_NAME})

project (${MAIN_NAME})

set (${MAIN_NAME}_VERSION_MAJOR 0)
set (${MAIN_NAME}_VERSION_MINOR 0)

configure_file (
    "${PROJECT_SOURCE_DIR}/template/config.h.in"
    "${PROJECT_SOURCE_DIR}/src/${MAIN_NAME}/config.h"
)

message ("COMPILER: ${CMAKE_CXX_COMPILER_ID}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set (MAIN_LINK -Wl,-force_load ${MAIN_NAME_STATIC})
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set (MAIN_LINK -Wl,--whole-archive ${MAIN_NAME_STATIC} -Wl,--no-whole-archive)
endif ()

add_library (${MAIN_NAME_STATIC} STATIC ${PROJECT_SOURCE_DIR}/src/__test__.cpp)
add_library (${MAIN_NAME_SHARED} SHARED)
add_dependencies (${MAIN_NAME_SHARED} ${MAIN_NAME_STATIC})
set_target_properties (${MAIN_NAME_STATIC} PROPERTIES OUTPUT_NAME ${MAIN_NAME})
target_link_libraries (${MAIN_NAME_SHARED} ${MAIN_LINK})

include_directories (3rd)
include_directories (src)

set  (CMAKE_VERBOSE_MAKEFILE  ON)
#set (CMAKE_MACOSX_RPATH      1)

add_subdirectory (sample)
add_subdirectory (test)

