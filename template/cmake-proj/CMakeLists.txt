## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()

enable_language (C)
enable_language (CXX)

# ---------------
# CMake settings.
# ---------------

list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# User defined settings.
include (config.cmake)

# CMake extension script.
include (cmake/compile.cmake)
include (cmake/build.cmake)
include (cmake/util.cmake)

exists_define_or_die (MAIN_NAME)

string (TOUPPER ${MAIN_NAME} MAIN_NAME_UPPER)
string (TIMESTAMP _timestamp "%Y-%m-%d_%H%M%S")
set (VERSION_TWEAK "${_timestamp}")
set (VERSION   "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set (SOVERSION "${VERSION_MAJOR}")

# Third-party libraries.
set (THIRD_BIN "${THIRD_PREFIX}/bin")
set (THIRD_INC "${THIRD_PREFIX}/include")
set (THIRD_LIB "${THIRD_PREFIX}/lib")

if (IS_DIRECTORY ${THIRD_BIN})
    set (ENV{PATH} "${THIRD_BIN}${PATH_SEPARATOR}$ENV{PATH}")
endif ()
if (IS_DIRECTORY ${THIRD_INC})
    set (ENV{CPATH} "${THIRD_INC}${PATH_SEPARATOR}$ENV{CPATH}")
endif ()
if (IS_DIRECTORY ${THIRD_LIB})
    set (ENV{LIBRARY_PATH} "${THIRD_LIB}${PATH_SEPARATOR}$ENV{LIBRARY_PATH}")
endif ()

list (INSERT CMAKE_PROGRAM_PATH 0 "${THIRD_BIN}")
list (INSERT CMAKE_LIBRARY_PATH 0 "${THIRD_LIB}")
list (INSERT CMAKE_PROGRAM_PATH 0 "${PROJECT_SOURCE_DIR}")
list (INSERT CMAKE_LIBRARY_PATH 0 "${PROJECT_SOURCE_DIR}")

#set (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
#set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set  (CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")

# Extension variables.
set (ZLIB_ROOT     "${THIRD_PREFIX}")
set (GTEST_ROOT    "${THIRD_PREFIX}")
set (GFLAGS_ROOT   "${THIRD_PREFIX}")
set (GLOG_ROOT     "${THIRD_PREFIX}")
set (LMDB_ROOT     "${THIRD_PREFIX}")
set (HDF5_ROOT     "${THIRD_PREFIX}")
set (OPENBLAS_ROOT "${THIRD_PREFIX}")
set (UV_ROOT       "${THIRD_PREFIX}")
set (SNAPPY_ROOT   "${THIRD_PREFIX}")
set (LEVELDB_ROOT  "${THIRD_PREFIX}")
set (BOOST_ROOT    "${THIRD_PREFIX}")
set (TBAG_ROOT     "${THIRD_PREFIX}")
set (wxWidgets_ROOT_DIR "${THIRD_PREFIX}")

if (WIN32 AND NOT CYGWIN AND NOT MSYS)
    # Force setting: wxWidgets_FIND_STYLE = unix
    set (CMAKE_CROSSCOMPILING YES)
endif ()

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
test_find_package  (tbag      Tbag)

# Compiler extension.
test_find_package (CUDA      CUDA)
test_find_package (CUDNN     CUDNN)
test_find_package (OBFUSCATE CxxObfuscator)

# Continuous Integration.
test_find_package (DOXYGEN Doxygen)

# wxWidgets setting.
if (wxWidgets_FOUND)
    if (WIN32 AND NOT CYGWIN AND NOT MSYS)
        convert_msys_to_windows_path (_convert_path "${wxWidgets_INCLUDE_DIRS}")
        set (wxWidgets_INCLUDE_DIRS ${_convert_path})
    endif ()

    # compile time link dirs, useful for rpath on UNIX.
    # Typically an empty string in WIN32 environment.
    if (UNIX)
        set (ENV{LIBRARY_PATH} "${wxWidgets_LIBRARY_DIRS}${PATH_SEPARATOR}$ENV{LIBRARY_PATH}")
        list (INSERT CMAKE_LIBRARY_PATH 0 "${wxWidgets_LIBRARY_DIRS}")
    endif ()
endif ()

# -------------------
# Configure settings.
# -------------------

set (TEMPLATE_PREFIX "${PROJECT_SOURCE_DIR}/template")
set (MAIN_DIR "${PROJECT_SOURCE_DIR}/${MAIN_NAME}")

set (IN_CONFIG    "${TEMPLATE_PREFIX}/config.h.in")
set (IN_DOXYFILE  "${TEMPLATE_PREFIX}/Doxyfile.in")

set (OUT_CONFIG   "${MAIN_DIR}/config.h")
set (OUT_DOXYFILE "${PROJECT_SOURCE_DIR}/Doxyfile")

configure_file (${IN_CONFIG}   ${OUT_CONFIG}   NEWLINE_STYLE UNIX)
configure_file (${IN_DOXYFILE} ${OUT_DOXYFILE} NEWLINE_STYLE UNIX)

# -----------
# Properties.
# -----------

#set (BUILD_SHARED_LIBS                ON) # User's variable.
#set (CMAKE_MACOSX_RPATH                1) # CMP0042
#set (CMAKE_BUILD_TYPE            Release) # User's variable.
set  (CMAKE_CXX_EXTENSIONS            OFF) # Change: -std=gnu++11 -> -std=c++11
set  (CMAKE_CXX_STANDARD               11) # C++ standard 11
set  (CMAKE_POSITION_INDEPENDENT_CODE  ON) # -fPIC

set  (CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} -Wall")
#set (CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}")
#set (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")
#set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")

# Build type: Debug, Release, RelWithDebInfo, MinSizeRel
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions (-DDEBUG)
else ()
    add_definitions (-DNDEBUG -DRELEASE)
endif ()

include_directories (${PROJECT_SOURCE_DIR})
if (IS_DIRECTORY ${THIRD_INC})
    include_directories (${THIRD_INC})
endif ()

link_directories (${PROJECT_SOURCE_DIR})
if (IS_DIRECTORY ${THIRD_LIB})
    link_directories (${THIRD_LIB})
endif ()

# ------------
# Sub-project.
# ------------

find_project (_lib_proj_list _test_proj_list _exe_proj_list)

message ("** Find library project: ${_lib_proj_list}")
message ("** Find test project: ${_test_proj_list}")
message ("** Find executable project: ${_exe_proj_list}")

default_build ("${_lib_proj_list}" "${_test_proj_list}" "${_exe_proj_list}")

# -----------------------
# Continuous Integration.
# -----------------------

doxygen_generate ("${OUT_DOXYFILE}")

#print_os ()
#print_compiler_id ()

