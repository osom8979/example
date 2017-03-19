cmake_minimum_required (VERSION 2.8)

set (PROJECT_NAME wxGoogleMap)
project (${PROJECT_NAME})

# Setup the THIRD_PREFIX variable.
string (REPLACE "\\" "/" __tparty_home_path "$ENV{TPARTY_HOME}/local")
if (IS_DIRECTORY "${__tparty_home_path}")
    list (INSERT CMAKE_PROGRAM_PATH 0 "${__tparty_home_path}/bin")
    list (INSERT CMAKE_INCLUDE_PATH 0 "${__tparty_home_path}/include")
    list (INSERT CMAKE_LIBRARY_PATH 0 "${__tparty_home_path}/lib")
    set (wxWidgets_ROOT_DIR "${__tparty_home_path}")
else ()
    message (STATUS "Not found tparty local directory: ${__tparty_home_path}")
endif ()

find_package (wxWidgets REQUIRED base core webview)
include_directories (${wxWidgets_INCLUDE_DIRS})
include_directories (${CMAKE_SOURCE_DIR})

foreach (__define_cursor ${wxWidgets_DEFINITIONS})
    add_definitions (-D${__define_cursor})
endforeach ()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${wxWidgets_CXX_FLAGS}")

set (SOURCE_FILES main.cpp wxGoogleMap.cpp jsoncpp.cpp)

add_executable (${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries (${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})

