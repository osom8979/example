cmake_minimum_required (VERSION 2.8)

set (PROJECT_NAME dsn)
project (${PROJECT_NAME})

# Version.
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_PATCH 0)
mark_as_advanced (VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

set (VERSION   "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set (SOVERSION "${VERSION_MAJOR}")

# Tweak version.
string (TIMESTAMP __timestamp "%Y-%m-%d_%H%M%S")
set (VERSION_TWEAK  "${__timestamp}")
unset (__timestamp)

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

if (WIN32)
    if (MSVC)
        set (CMAKE_EXE_LINKER_FLAGS /SUBSYSTEM:WINDOWS)
    elseif (MINGW)
        set (CMAKE_EXE_LINKER_FLAGS -mwindows)
        set (CMAKE_CXX_FLAGS -Wno-deprecated-declarations)
    endif ()
endif ()

find_package (wxWidgets REQUIRED net core base)
include_directories (${wxWidgets_INCLUDE_DIRS})
include_directories (${CMAKE_SOURCE_DIR})

foreach (__define_cursor ${wxWidgets_DEFINITIONS})
    add_definitions (-D${__define_cursor})
endforeach ()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${wxWidgets_CXX_FLAGS}")

file (GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/dsn/*.cpp")

add_executable (${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries (${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})

install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

