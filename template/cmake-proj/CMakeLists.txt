## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

enable_language (C)
enable_language (CXX)

# ---------------
# CMake settings.
# ---------------

set (_CMAKE_BUILD_TYPE_DEBUG           "Debug")
set (_CMAKE_BUILD_TYPE_RELEASE         "Release")
set (_CMAKE_BUILD_TYPE_RELWITHDEBINFO  "RelWithDebInfo")
set (_CMAKE_BUILD_TYPE_MINSIZEREL      "MinSizeRel")

 set (CMAKE_MACOSX_RPATH 1) # CMP0042
#set (CMAKE_VERBOSE_MAKEFILE ON)
 set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall")
 set (BUILD_SHARED_LIBS ON)
#set (CMAKE_BUILD_TYPE "Debug")
#set (CMAKE_BUILD_TYPE "Release")

list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# User defined settings.
include (config.cmake)

# CMake extension script.
include (cmake/library.cmake)
include (cmake/compile.cmake)
include (cmake/build.cmake)
include (cmake/util.cmake)

exists_define_or_die (MAIN_NAME)
exists_define_or_die (LIST_OF_LIBRARIES)

string (TIMESTAMP _timestamp "%Y-%m-%d_%H%M%S")
set (VERSION_TWEAK "${_timestamp}")
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Compiler extension.
set (USE_CUDA       ON  CACHE BOOL "Use the nvidia-cuda.")
set (USE_OBFUSCATE  ON  CACHE BOOL "Use the obfuscate.")

# Third-party libraries.
set (THIRD_PREFIX  "${PROJECT_SOURCE_DIR}/3rd")
set (THIRD_BIN     "${THIRD_PREFIX}/bin")
set (THIRD_INC     "${THIRD_PREFIX}/include")
set (THIRD_LIB     "${THIRD_PREFIX}/lib")

set (ENV{PATH}         "${THIRD_BIN}${PATH_SEPARATOR}$ENV{PATH}")
set (ENV{CPATH}        "${THIRD_INC}${PATH_SEPARATOR}$ENV{CPATH}")
set (ENV{LIBRARY_PATH} "${THIRD_LIB}${PATH_SEPARATOR}$ENV{LIBRARY_PATH}")

list (INSERT CMAKE_PROGRAM_PATH 0 "${THIRD_BIN}")
list (INSERT CMAKE_LIBRARY_PATH 0 "${THIRD_LIB}")
list (INSERT CMAKE_PROGRAM_PATH 0 "${PROJECT_SOURCE_DIR}")
list (INSERT CMAKE_LIBRARY_PATH 0 "${PROJECT_SOURCE_DIR}")

set (CMAKE_INSTALL_RPATH "." "${THIRD_LIB}")
set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# [WARNING] Don't move this code.
# Download & Build, third-party project.
build_libraries ("${LIST_OF_LIBRARIES}" "${THIRD_PREFIX}")

# Extension variables.
set (ZLIB_ROOT         "${THIRD_PREFIX}")
set (GTEST_ROOT        "${THIRD_PREFIX}")
set (GFLAGS_ROOT_DIR   "${THIRD_PREFIX}")
set (GLOG_ROOT_DIR     "${THIRD_PREFIX}")
set (LMDB_ROOT_DIR     "${THIRD_PREFIX}")
set (HDF5_ROOT         "${THIRD_PREFIX}")
set (OPENBLAS_ROOT_DIR "${THIRD_PREFIX}")
set (UV_ROOT_DIR       "${THIRD_PREFIX}")
set (SNAPPY_ROOT_DIR   "${THIRD_PREFIX}")
set (LEVELDB_ROOT_DIR  "${THIRD_PREFIX}")
set (BOOST_ROOT        "${THIRD_PREFIX}")

# Test packages.
test_find_package  (zlib      ZLIB)
test_find_package  (gtest     GTest)
test_find_package  (gflags    GFlags)
test_find_package  (glog      GLog)
test_find_package  (protobuf  Protobuf)
test_find_package  (lmdb      LMDB)
test_find_package  (hdf5      HDF5)
test_find_package  (openblas  OpenBLAS)
test_find_package  (libuv     UV)
test_find_package  (snappy    Snappy)
test_find_package  (leveldb   LevelDB)
test_find_package  (libpng    PNG)
test_find_package  (boost     Boost)
#test_find_package (ffmpeg    FFmpeg)
test_find_package  (wxwidgets wxWidgets)
#test_find_package (sdl2      SDL2)

# -------------------
# Configure settings.
# -------------------

set (TEMPLATE_PREFIX "${PROJECT_SOURCE_DIR}/template")
set (MAIN_DIR "${PROJECT_SOURCE_DIR}/${MAIN_NAME}")

set (IN_PREFIX    "${TEMPLATE_PREFIX}/prefix.h.in")
set (IN_CONFIG    "${TEMPLATE_PREFIX}/config.h.in")
set (IN_DOXYFILE  "${TEMPLATE_PREFIX}/Doxyfile.in")

set (OUT_PREFIX   "${MAIN_DIR}/prefix.h")
set (OUT_CONFIG   "${MAIN_DIR}/config.h")
set (OUT_DOXYFILE "${PROJECT_SOURCE_DIR}/Doxyfile")

configure_file (${IN_PREFIX}   ${OUT_PREFIX}   NEWLINE_STYLE UNIX)
configure_file (${IN_CONFIG}   ${OUT_CONFIG}   NEWLINE_STYLE UNIX)
configure_file (${IN_DOXYFILE} ${OUT_DOXYFILE} NEWLINE_STYLE UNIX)

# -----------
# Properties.
# -----------

if (CMAKE_BUILD_TYPE MATCHES "${_CMAKE_BUILD_TYPE_DEBUG}")
    add_definitions (-DDEBUG)
else ()
    add_definitions (-DRELEASE)
endif ()

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${THIRD_INC})

link_directories (${PROJECT_SOURCE_DIR})
link_directories (${THIRD_LIB})

# ------------
# Sub-project.
# ------------

find_project (_lib_proj_list _test_proj_list _exe_proj_list)

message ("** Find library project: ${_lib_proj_list}")
message ("** Find test project: ${_test_proj_list}")
message ("** Find executable project: ${_exe_proj_list}")

default_build ("${_lib_proj_list}" "${_test_proj_list}" "${_exe_proj_list}")

#print_os ()
#print_compiler_id ()

