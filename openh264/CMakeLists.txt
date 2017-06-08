cmake_minimum_required (VERSION 2.8)

set (NAME openh264)
project (${NAME})

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set (SRCS main.cpp)
add_executable (${PROJECT_NAME} ${SRCS})

set (LIBS -lopenh264)

target_include_directories (${NAME} PRIVATE $ENV{TPARTY_HOME}/local/include)
target_compile_options (${NAME} PRIVATE -std=c++11)
target_link_libraries (${NAME} PRIVATE -L$ENV{TPARTY_HOME}/local/lib ${LIBS})


