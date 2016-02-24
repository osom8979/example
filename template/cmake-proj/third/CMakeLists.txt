## CMake third build script.

exists_define_or_die (THIRD_NAME)
exists_define_or_die (THIRD_PREFIX)

project (${THIRD_NAME})

set (THIRD_DIR        "${CMAKE_CURRENT_SOURCE_DIR}")
set (BUILD_DIR        "${THIRD_DIR}/build")
set (THIRD_STATIC_LIB "${THIRD_DIR}/libthird.a")
set (THIRD_LIBS)

add_library (THIRD_OBJ OBJECT "third.c")
add_library (${THIRD_NAME} STATIC $<TARGET_OBJECTS:THIRD_OBJ>)

# ----------------------
# Third-party libraries.
# ----------------------

if (USE_ZLIB)
    message ("** Use the zlib.")

    set  (ZLIB_LIB "${THIRD_PREFIX}/lib/libz.a")
    list (APPEND THIRD_LIBS "${ZLIB_LIB}")

    if (EXISTS ${ZLIB_LIB})
        message ("** Exists zlib library: ${ZLIB_LIB}")
    else ()
        message ("** Install zlib library.")
        install_zlib ("${BUILD_DIR}" "${THIRD_PREFIX}"
            "${THIRD_PREFIX}/include" "${THIRD_PREFIX}/lib")
    endif ()
endif()

if (USE_PROTOBUF)
    message ("** Use the google-protocol-buffers.")

    set  (PROTOBUF_LIB "${THIRD_PREFIX}/lib/libprotobuf.a")
    set  (PROTOBUF_EXE "${THIRD_PREFIX}/bin/protoc")
    list (APPEND THIRD_LIBS "${PROTOBUF_LIB}")

    if (EXISTS ${PROTOBUF_LIB} AND EXISTS ${PROTOBUF_EXE})
        message ("** Exists protobuf library: ${PROTOBUF_LIB}")
    else ()
        message ("** Install protobuf library.")
        install_protobuf ("${BUILD_DIR}" "${THIRD_PREFIX}"
            "${THIRD_PREFIX}/include" "${THIRD_PREFIX}/lib")
    endif ()
endif ()

