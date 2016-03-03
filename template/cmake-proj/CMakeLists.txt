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
include (cmake/3rd.cmake)
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

set (ENV{PATH}             "${THIRD_BIN}${PATH_SEPARATOR}$ENV{PATH}")
set (ENV{CPATH}            "${THIRD_INC}${PATH_SEPARATOR}$ENV{CPATH}")
set (ENV{LIBRARY_PATH}     "${THIRD_LIB}${PATH_SEPARATOR}$ENV{LIBRARY_PATH}")
set (ENV{LD_LIBRARY_PATH}  "${THIRD_LIB}${PATH_SEPARATOR}$ENV{LD_LIBRARY_PATH}")

list (INSERT CMAKE_PROGRAM_PATH 0 "${THIRD_BIN}")
list (INSERT CMAKE_LIBRARY_PATH 0 "${THIRD_LIB}")
list (INSERT CMAKE_PROGRAM_PATH 0 "${PROJECT_SOURCE_DIR}")
list (INSERT CMAKE_LIBRARY_PATH 0 "${PROJECT_SOURCE_DIR}")

# [WARNING] Don't move this code.
# Download & Build, third-party project.
build_3rd ("${LIST_OF_LIBRARIES}" "${THIRD_PREFIX}")

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

set (_PROJECT_PREFIX      "project")
set (_PROJECT_SUFFIX      "cmake")
SET (_PROJECT_NAME        "${_PROJECT_PREFIX}.${_PROJECT_SUFFIX}")
SET (_PROJECT_NAME_REGEX  "${_PROJECT_PREFIX}\\.${_PROJECT_SUFFIX}")

set (_lib_proj_glob  "^lib[^/]+/${_PROJECT_NAME_REGEX}$")
set (_test_proj_glob "^test[^/]+/${_PROJECT_NAME_REGEX}$")
set (_exe_proj_glob  "^[^/]+/${_PROJECT_NAME_REGEX}$")

file (GLOB_RECURSE _find_cmakelists RELATIVE "${PROJECT_SOURCE_DIR}" "*/${_PROJECT_NAME}")
# Find library project.
list_regex (_find_lib_proj "${_lib_proj_glob}" "${_find_cmakelists}")

# Find test executable project.
list (LENGTH _find_lib_proj _find_lib_proj_length)
if (${_find_lib_proj_length} GREATER 0)
    list (REMOVE_ITEM _find_cmakelists ${_find_lib_proj})
endif ()
list_regex (_find_test_proj "${_test_proj_glob}" "${_find_cmakelists}")

# Find executable project.
list (LENGTH _find_test_proj _find_test_proj_length)
if (${_find_test_proj_length} GREATER 0)
    list (REMOVE_ITEM _find_cmakelists ${_find_test_proj})
endif ()
list_regex (_find_exe_proj "${_exe_proj_glob}" "${_find_cmakelists}")

string (REPLACE "/${_PROJECT_NAME}" "" _lib_proj_list  "${_find_lib_proj}")
string (REPLACE "/${_PROJECT_NAME}" "" _test_proj_list "${_find_test_proj}")
string (REPLACE "/${_PROJECT_NAME}" "" _exe_proj_list  "${_find_exe_proj}")

message ("** Find library project: ${_lib_proj_list}")
message ("** Find test project: ${_test_proj_list}")
message ("** Find executable project: ${_exe_proj_list}")

#print_os ()
#print_compiler_id ()

default_build ("${_lib_proj_list}" "${_test_proj_list}" "${_exe_proj_list}")

