## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

enable_language (C)
enable_language (CXX)

include (cmake/link.cmake)
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
set (SAMPLE_NAME "sample"  CACHE STRING  "Sample program name string.")
set (TEST_NAME   "tester"  CACHE STRING  "Test program name string.")

set (BRIEF "CMake project template" CACHE STRING "Project brief.")

set (USE_PROTOBUF  ON CACHE BOOL "Use the google-protocol-buffers.")
set (USE_CUDA      ON CACHE BOOL "Use the nvidia-cuda.")
set (USE_OBFUSCATE ON CACHE BOOL "Use the obfuscate.")

set (VERSION_MAJOR  0)
set (VERSION_MINOR  0)
set (VERSION_PATCH  0)
set (VERSION_TWEAK  0)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}")

set (VERSION_PACKET_MAJOR  0)
set (VERSION_PACKET_MINOR  0)
set (VERSION_RELEASE       0)

# -------------------
# Configure settings.
# -------------------

set (TEMPLATE_DIR "${PROJECT_SOURCE_DIR}/template")
set (MAIN_SRC_DIR "${PROJECT_SOURCE_DIR}/src/${MAIN_NAME}")

set (IN_PREFIX    "${TEMPLATE_DIR}/prefix.h.in")
set (IN_CONFIG    "${TEMPLATE_DIR}/config.h.in")
set (IN_DOXYFILE  "${TEMPLATE_DIR}/Doxyfile.in")

set (OUT_PREFIX   "${MAIN_SRC_DIR}/prefix.h")
set (OUT_CONFIG   "${MAIN_SRC_DIR}/config.h")
set (OUT_DOXYFILE "${PROJECT_SOURCE_DIR}/Doxyfile")

configure_file (${IN_PREFIX}   ${OUT_PREFIX}   NEWLINE_STYLE UNIX)
configure_file (${IN_CONFIG}   ${OUT_CONFIG}   NEWLINE_STYLE UNIX)
configure_file (${IN_DOXYFILE} ${OUT_DOXYFILE} NEWLINE_STYLE UNIX)

# -----------
# Properties.
# -----------

include_directories (third)
include_directories (proto)
include_directories (cuda)
include_directories (src)

# ---------
# Children.
# ---------

add_subdirectory (third)
add_subdirectory (src)
add_subdirectory (sample)
add_subdirectory (test)

