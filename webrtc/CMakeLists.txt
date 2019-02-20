# libsourcey WebRTC Video Server

cmake_minimum_required (VERSION 2.8)

set (PROJECT_NAME webrtc)
project (${PROJECT_NAME})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-rtti -fvisibility=hidden")

set (JSONCPP_FILES "${CMAKE_SOURCE_DIR}/jsoncpp/jsoncpp.cpp")
set (LIVE555_HELPER_FILES
        "${CMAKE_SOURCE_DIR}/live555helper/src/environment.cpp"
        "${CMAKE_SOURCE_DIR}/live555helper/src/mkvclient.cpp"
        "${CMAKE_SOURCE_DIR}/live555helper/src/rtspconnectionclient.cpp"
        "${CMAKE_SOURCE_DIR}/live555helper/src/sdpclient.cpp"
        "${CMAKE_SOURCE_DIR}/live555helper/src/SessionSink.cpp")
set (SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/webrtc-streamer/src/HttpServerRequestHandler.cpp"
        "${CMAKE_SOURCE_DIR}/webrtc-streamer/src/PeerConnectionManager.cpp"
        "${CMAKE_SOURCE_DIR}/webrtc-streamer/src/filecapturer.cpp"
        "${CMAKE_SOURCE_DIR}/webrtc-streamer/src/main.cpp"
        "${CMAKE_SOURCE_DIR}/webrtc-streamer/src/rtspaudiocapturer.cpp"
        "${CMAKE_SOURCE_DIR}/webrtc-streamer/src/rtspvideocapturer.cpp"
        "${CMAKE_SOURCE_DIR}/webrtc-streamer/src/screencapturer.cpp")

add_executable (${PROJECT_NAME} ${SOURCE_FILES} ${JSONCPP_FILES} ${LIVE555_HELPER_FILES})

set (PROJECT_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/jsoncpp
        ${CMAKE_SOURCE_DIR}/live555helper/inc
        ${CMAKE_SOURCE_DIR}/webrtc-streamer/inc)

set (WEBRTC_PREFIX /usr/local/c2core/webrtc)
set (WEBRTC_INCLUDE_DIRS
        ${WEBRTC_PREFIX}/include
        ${WEBRTC_PREFIX}/include/third_party/abseil-cpp
        ${WEBRTC_PREFIX}/include/third_party/jsoncpp/source/include
        ${WEBRTC_PREFIX}/include/third_party/libyuv/include)
        #${WEBRTC_PREFIX}/include/third_party/libvpx/source/libvpx/third_party/libyuv/include)
set (WEBRTC_LIBRARIES
        -Wl,-force_load,${WEBRTC_PREFIX}/lib/libwebrtc.a
        -Wl,-force_load,${WEBRTC_PREFIX}/lib/libcommand_line_parser.a
        "-framework Foundation"
        "-framework AVFoundation"
        "-framework CoreAudio"
        "-framework AudioToolbox"
        "-framework CoreFoundation"
        "-framework CoreGraphics"
        "-framework CoreMedia"
        "-framework CoreVideo"
        "-framework IOSurface"
        "-framework Cocoa")
#"-framework IOKit"
#"-framework Security"
#"-framework SystemConfiguration"
#"-framework ApplicationServices"
#"-framework OpenGL"
#"-framework VideoToolbox"

macro (simple_find_library __var __header __lib)
    find_path (${__var}_INC NAMES ${__header})
    find_library (${__var}_LIB NAMES ${__lib})
endmacro ()

simple_find_library (avformat   libavformat/avformat.h     avformat)
simple_find_library (avutil     libavutil/avutil.h         avutil)
simple_find_library (avcodec    libavcodec/avcodec.h       avcodec)
simple_find_library (swscale    libswscale/swscale.h       swscale)
simple_find_library (swresample libswresample/swresample.h swresample)
simple_find_library (avfilter   libavfilter/avfilter.h     avfilter)
simple_find_library (avdevice   libavdevice/avdevice.h     avdevice)

set (FFMPEG_INCLUDE_DIR
        ${avformat_INC} ${avutil_INC} ${avcodec_INC} ${swscale_INC}
        ${swresample_INC} ${avfilter_INC} ${avdevice_INC})
set (FFMPEG_LIBRARIES
        ${avformat_LIB} ${avutil_LIB} ${avcodec_LIB} ${swscale_LIB}
        ${swresample_LIB} ${avfilter_LIB} ${avdevice_LIB})
list (REMOVE_DUPLICATES FFMPEG_INCLUDE_DIR)
list (REMOVE_DUPLICATES FFMPEG_LIBRARIES)

message (STATUS "FFmpeg Include Dir: ${FFMPEG_INCLUDE_DIR}")
message (STATUS "FFmpeg Libraries: ${FFMPEG_LIBRARIES}")

simple_find_library (BasicUsageEnvironment BasicUsageEnvironment/BasicUsageEnvironment.hh BasicUsageEnvironment)
simple_find_library (UsageEnvironment UsageEnvironment/UsageEnvironment.hh UsageEnvironment)
simple_find_library (Groupsock groupsock/Groupsock.hh groupsock)
simple_find_library (LiveMedia liveMedia/liveMedia.hh liveMedia)

set (LIVE555_INCLUDE_DIR
        ${BasicUsageEnvironment_INC}/BasicUsageEnvironment
        ${UsageEnvironment_INC}/UsageEnvironment
        ${Groupsock_INC}/groupsock
        ${LiveMedia_INC}/liveMedia)
set (LIVE555_LIBRARIES
        ${BasicUsageEnvironment_LIB}
        ${UsageEnvironment_LIB}
        ${Groupsock_LIB}
        ${LiveMedia_LIB})
list (REMOVE_DUPLICATES LIVE555_INCLUDE_DIR)
list (REMOVE_DUPLICATES LIVE555_LIBRARIES)

message (STATUS "Live555 Include Dir: ${LIVE555_INCLUDE_DIR}")
message (STATUS "Live555 Libraries: ${LIVE555_LIBRARIES}")

# cmake -DCMAKE_INSTALL_PREFIX=$PWD/install -DCIVETWEB_ENABLE_CXX=ON ..
# make install
set (CIVETWEB_PREFIX /Users/zer0/Project/civetweb/cmake-build-debug/install)
set (CIVETWEB_INCLUDE_DIRS ${CIVETWEB_PREFIX}/include)
set (CIVETWEB_LIBRARIES
        -Wl,-force_load,${CIVETWEB_PREFIX}/lib/libcivetweb.a
        -Wl,-force_load,${CIVETWEB_PREFIX}/lib/libcivetweb-cpp.a)

#add_library (civetweb STATIC
#        ${CMAKE_SOURCE_DIR}/civetweb/src/civetweb.c
#        ${CMAKE_SOURCE_DIR}/civetweb/src/CivetServer.cpp)
#set (CIVETWEB_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/civetweb/include)
#target_include_directories (civetweb PUBLIC
#        ${CIVETWEB_INCLUDE_DIRS})
#add_dependencies (${PROJECT_NAME} civetweb)
#set (CIVETWEB_LIBRARIES civetweb)

target_compile_definitions (${PROJECT_NAME} PRIVATE
        WEBRTC_MAC
        WEBRTC_POSIX
        HAVE_LIVE555
        HAVE_JPEG
        VERSION="3.0.0")
target_include_directories (${PROJECT_NAME} PRIVATE
        ${PROJECT_INCLUDE_DIRS}
        ${WEBRTC_INCLUDE_DIRS}
        ${FFMPEG_INCLUDE_DIR}
        ${LIVE555_INCLUDE_DIR}
        ${CIVETWEB_INCLUDE_DIRS})
target_link_libraries (${PROJECT_NAME} PRIVATE
        ${WEBRTC_LIBRARIES}
        ${FFMPEG_LIBRARIES}
        ${LIVE555_LIBRARIES}
        ${CIVETWEB_LIBRARIES})

