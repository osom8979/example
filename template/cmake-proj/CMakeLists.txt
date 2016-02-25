## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

enable_language (C)
enable_language (CXX)

include (cmake/link.cmake)
include (cmake/module.cmake)
include (cmake/util.cmake)
include (cmake/protobuf.cmake)
include (cmake/zlib.cmake)

# ---------------
# CMake settings.
# ---------------

 set (CMAKE_MACOSX_RPATH 1) # CMP0042
#set (CMAKE_VERBOSE_MAKEFILE ON)
 set (BUILD_SHARED_LIBS ON)
 set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC -Wall")
#set (CMAKE_BUILD_TYPE "Debug")
#set (CMAKE_BUILD_TYPE "Release")

#print_os ()
#print_compiler_id ()

set (MAIN_NAME   "main"    CACHE STRING  "Main library name string.")
set (THIRD_NAME  "third"   CACHE STRING  "Third-party library name string.")
set (SAMPLE_NAME "sample"  CACHE STRING  "Sample program name string.")
set (TEST_NAME   "tester"  CACHE STRING  "Test program name string.")

set (BRIEF "CMake project template" CACHE STRING "Project brief.")

set (USE_ZLIB      ON  CACHE BOOL "Use the zlib.")
set (USE_PROTOBUF  ON  CACHE BOOL "Use the google-protocol-buffers.")
set (USE_CUDA      ON  CACHE BOOL "Use the nvidia-cuda.")
set (USE_OBFUSCATE OFF CACHE BOOL "Use the obfuscate.")

set (VERSION_MAJOR  0)
set (VERSION_MINOR  0)
set (VERSION_PATCH  0)
set (VERSION_TWEAK  0)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}")

set (VERSION_PACKET_MAJOR  0)
set (VERSION_PACKET_MINOR  0)
set (VERSION_RELEASE       0)

set (THIRD_PREFIX "${PROJECT_SOURCE_DIR}/third")
set (THIRD_BIN    "${THIRD_PREFIX}/bin")
set (THIRD_INC    "${THIRD_PREFIX}/include")
set (THIRD_LIB    "${THIRD_PREFIX}/lib")
set (BUILD_DIR    "${THIRD_PREFIX}/build")

if (NOT EXISTS ${BUILD_DIR})
    file (MAKE_DIRECTORY ${BUILD_DIR})
endif ()

add_subdirectory (third)

list (INSERT CMAKE_PROGRAM_PATH 0 "${THIRD_BIN}")
list (INSERT CMAKE_LIBRARY_PATH 0 "${THIRD_LIB}")

# -------
# Module.
# -------

if (USE_PROTOBUF)
    find_extension_and_assign_module ("src" "proto" "Protobuf")
endif ()
if (USE_CUDA)
    find_extension_and_assign_module ("src" "cu" "CUDA")
endif ()

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

include_directories (src)
include_directories (${THIRD_INC})

link_directories (${THIRD_LIB})

# ---------
# Children.
# ---------

add_subdirectory (src)
add_subdirectory (sample)
add_subdirectory (test)

