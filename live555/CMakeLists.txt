cmake_minimum_required(VERSION 3.13)
project(rtsp_server CXX)

set(CMAKE_CXX_STANDARD 14)

# add_definitions (-DSFML_STATIC)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

include_directories (
        /usr/local/c2core/include
        /usr/local/include
        /usr/local/include/BasicUsageEnvironment
        /usr/local/include/UsageEnvironment
        /usr/local/include/groupsock
        /usr/local/include/liveMedia)
link_directories (
        /usr/local/c2core/lib
        /usr/local/lib)

file (GLOB_RECURSE SRCS "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(rtsp_server ${SRCS})
target_link_libraries (rtsp_server PRIVATE
        -lavcodec -lavformat -lavutil -lavfilter -lavdevice -lswscale
        -lBasicUsageEnvironment -lUsageEnvironment -lliveMedia -lgroupsock)
