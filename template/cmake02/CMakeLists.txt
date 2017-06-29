cmake_minimum_required (VERSION 2.8)

set (PROJECT_NAME cmake02)
project (${PROJECT_NAME})

# Setup the THIRD_PREFIX variable.
string (REPLACE "\\" "/" __tparty_home_path "$ENV{TPARTY_HOME}/local")
if (IS_DIRECTORY "${__tparty_home_path}")
    set (THIRD_PREFIX  "${__tparty_home_path}" CACHE PATH "3rd-party prefix directory.")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${THIRD_PREFIX}/include")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${THIRD_PREFIX}/lib")
else ()
    message (STATUS "Not found tparty local directory: ${__tparty_home_path}")
endif ()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

set (SOURCE_FILES main.cpp)
add_executable (${PROJECT_NAME} ${SOURCE_FILES})

