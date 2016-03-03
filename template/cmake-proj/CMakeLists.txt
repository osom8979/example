## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

enable_language (C)
enable_language (CXX)

# ---------------
# CMake settings.
# ---------------

set (CMAKE_BUILD_TYPE_DEBUG           "Debug")
set (CMAKE_BUILD_TYPE_RELEASE         "Release")
set (CMAKE_BUILD_TYPE_RELWITHDEBINFO  "RelWithDebInfo")
set (CMAKE_BUILD_TYPE_MINSIZEREL      "MinSizeRel")

 set (CMAKE_MACOSX_RPATH 1) # CMP0042
#set (CMAKE_VERBOSE_MAKEFILE ON)
 set (BUILD_SHARED_LIBS ON)
 set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC -Wall")
#set (CMAKE_BUILD_TYPE "Debug")
#set (CMAKE_BUILD_TYPE "Release")

set (MAIN_NAME   "main"    CACHE STRING  "Main library name string.")
set (THIRD_NAME  "third"   CACHE STRING  "Third-party library name string.")
set (SAMPLE_NAME "sample"  CACHE STRING  "Sample program name string.")
set (TEST_NAME   "tester"  CACHE STRING  "Test program name string.")

set (MAIN_BRIEF  "-"          CACHE STRING  "Project brief string.")
set (MAIN_AUTHOR "Anonymous"  CACHE STRING  "Project author.")
set (MAIN_EMAIL  "-"          CACHE STRING  "E-Mail address.")

set (USE_ZLIB      OFF CACHE BOOL "Use the zlib.")
set (USE_PROTOBUF  ON  CACHE BOOL "Use the google-protocol-buffers.")
set (USE_CUDA      ON  CACHE BOOL "Use the nvidia-cuda.")
set (USE_OBFUSCATE ON  CACHE BOOL "Use the obfuscate.")

set (VERSION_MAJOR  0)
set (VERSION_MINOR  0)
set (VERSION_PATCH  0)
set (VERSION_TWEAK  0)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}")

set (VERSION_PACKET_MAJOR  0)
set (VERSION_PACKET_MINOR  0)
set (VERSION_RELEASE       0)

set (MAIN_SRC_PREFIX        "${PROJECT_SOURCE_DIR}/src")
set (MAIN_SRC_NAME_PREFIX   "${MAIN_SRC_PREFIX}/${MAIN_NAME}")
set (SAMPLE_SRC_PREFIX      "${PROJECT_SOURCE_DIR}/sample")
set (TEST_SRC_PREFIX        "${PROJECT_SOURCE_DIR}/test")
set (TEMPLATE_PREFIX        "${PROJECT_SOURCE_DIR}/template")
set (THIRD_PREFIX           "${PROJECT_SOURCE_DIR}/third")

set (THIRD_BIN    "${THIRD_PREFIX}/bin")
set (THIRD_INC    "${THIRD_PREFIX}/include")
set (THIRD_LIB    "${THIRD_PREFIX}/lib")

list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

include (cmake/util.cmake)
include (cmake/protobuf.cmake)
include (cmake/zlib.cmake)
include (cmake/compile.cmake)

# [WARNING] Don't move this code.
# Download & Build, third-party project.
add_subdirectory (third)

# [WARNING] Don't move this code.
list (INSERT CMAKE_PROGRAM_PATH 0 "${THIRD_BIN}")
list (INSERT CMAKE_LIBRARY_PATH 0 "${THIRD_LIB}")

#print_os ()
#print_compiler_id ()

# -------------------
# Configure settings.
# -------------------

set (IN_PREFIX    "${TEMPLATE_PREFIX}/prefix.h.in")
set (IN_CONFIG    "${TEMPLATE_PREFIX}/config.h.in")
set (IN_DOXYFILE  "${TEMPLATE_PREFIX}/Doxyfile.in")

set (OUT_PREFIX   "${MAIN_SRC_NAME_PREFIX}/prefix.h")
set (OUT_CONFIG   "${MAIN_SRC_NAME_PREFIX}/config.h")
set (OUT_DOXYFILE "${PROJECT_SOURCE_DIR}/Doxyfile")

configure_file (${IN_PREFIX}   ${OUT_PREFIX}   NEWLINE_STYLE UNIX)
configure_file (${IN_CONFIG}   ${OUT_CONFIG}   NEWLINE_STYLE UNIX)
configure_file (${IN_DOXYFILE} ${OUT_DOXYFILE} NEWLINE_STYLE UNIX)

# -----------
# Properties.
# -----------

if (CMAKE_BUILD_TYPE MATCHES "${CMAKE_BUILD_TYPE_DEBUG}")
    add_definitions (-DDEBUG)
else ()
    add_definitions (-DRELEASE)
endif ()

include_directories (src)
include_directories (${THIRD_INC})
link_directories    (${THIRD_LIB})

# ---------
# Children.
# ---------

add_subdirectory (src)
add_subdirectory (sample)
add_subdirectory (test)

