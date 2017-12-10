cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()
#if (POLICY CMP0026)
#    # Disallow use of the LOCATION target property.
#    cmake_policy (SET CMP0026 NEW)
#endif ()

enable_language (C)
enable_language (CXX)

set (PROJECT_NAME cross)
project (${PROJECT_NAME})

## Find tbag library.
if (NOT TBAG_INCLUDE_DIR)
    find_path (TBAG_INCLUDE_DIR
               NAMES "libtbag/libtbag.h"
               PATHS "$ENV{TBAG_HOME}"
                     "$ENV{TPARTY_HOME}/local/include"
                     "/usr/local/include"
               PATH_SUFFIXES libtbag)
endif ()
if (NOT TBAG_LIBRARY_PATH)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
        set (__build_type_lower debug)
    else ()
        string (TOLOWER ${CMAKE_BUILD_TYPE} __build_type_lower)
    endif ()
    find_library (TBAG_LIBRARY_PATH
                  NAMES "tbag"
                  PATHS "$ENV{TBAG_HOME}/build"
                        "$ENV{TPARTY_HOME}/local/lib"
                        "$ENV{TBAG_HOME}/cmake-build-${__build_type_lower}"
                        "/usr/local/lib"
                  PATH_SUFFIXES Debug Release)
endif ()

if (DEFINED TBAG_INCLUDE_DIR)
    message (STATUS "Found tbag include dir: ${TBAG_INCLUDE_DIR}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${TBAG_INCLUDE_DIR}")
endif ()
if (DEFINED TBAG_LIBRARY_PATH)
    message (STATUS "Found tbag library: ${TBAG_LIBRARY_PATH}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${TBAG_LIBRARY_PATH}")

    if (CMAKE_C_COMPILER_ID STREQUAL GNU OR CMAKE_C_COMPILER_ID STREQUAL Clang)
        get_filename_component (TBAG_LIBRARY_DIR ${TBAG_LIBRARY_PATH} DIRECTORY)
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,${TBAG_LIBRARY_DIR}")
    endif ()
endif ()

# Setup the THIRD_PREFIX variable.
string (REPLACE "\\" "/" __tparty_home_path "$ENV{TPARTY_HOME}/local")
if (IS_DIRECTORY "${__tparty_home_path}")
    set (THIRD_PREFIX  "${__tparty_home_path}" CACHE PATH "3rd-party prefix directory.")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${THIRD_PREFIX}/include")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,${THIRD_PREFIX}/lib -L${THIRD_PREFIX}/lib")
else ()
    message (STATUS "Not found tparty local directory: ${__tparty_home_path}")
endif ()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

# OpenCV Libraries.
set (OPENCV_LIBS -lopencv_videostab
        -lopencv_videoio
        -lopencv_video
        -lopencv_superres
        -lopencv_stitching
        -lopencv_shape
        -lopencv_photo
        -lopencv_objdetect
        -lopencv_ml
        -lopencv_imgproc
        -lopencv_imgcodecs
        -lopencv_highgui
        -lopencv_flann
        -lopencv_features2d
        -lopencv_core
        -lopencv_calib3d)

set (SOURCE_FILES main.cpp)
add_executable (${PROJECT_NAME} ${SOURCE_FILES})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_OPENCV")
target_link_libraries (${PROJECT_NAME} PRIVATE ${OPENCV_LIBS})

set (CMAKE_INSTALL_RPATH_USE_LINK_PATH  TRUE)
set (CMAKE_INSTALL_RPATH     "${CMAKE_INSTALL_PREFIX}/lib")
set (CMAKE_INSTALL_NAME_DIR  "${CMAKE_INSTALL_PREFIX}/lib")

