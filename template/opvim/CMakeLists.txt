cmake_minimum_required (VERSION 2.8)

set (PROJECT_NAME main)
project (${PROJECT_NAME})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set (SOURCE_FILES main.cpp)
add_executable (${PROJECT_NAME} ${SOURCE_FILES})

