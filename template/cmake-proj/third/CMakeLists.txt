## CMake third build script.

exists_define_or_die (THIRD_PREFIX)

if (USE_ZLIB)
    message ("** Use the zlib.")

    set (ZLIB_LIB "${THIRD_PREFIX}/lib/libz.a")

    if (EXISTS ${ZLIB_LIB})
        message ("** Exists zlib library: ${ZLIB_LIB}")
    else ()
        message ("** Install zlib library.")
        install_zlib ("${BUILD_DIR}" "${THIRD_PREFIX}")
    endif ()
endif ()

if (USE_PROTOBUF)
    message ("** Use the google-protocol-buffers.")

    set (PROTOBUF_LIB "${THIRD_PREFIX}/lib/libprotobuf.a")
    set (PROTOBUF_EXE "${THIRD_PREFIX}/bin/protoc")

    if (EXISTS ${PROTOBUF_LIB} AND EXISTS ${PROTOBUF_EXE})
        message ("** Exists protobuf library: ${PROTOBUF_LIB}")
    else ()
        message ("** Install protobuf library.")
        install_protobuf ("${BUILD_DIR}" "${THIRD_PREFIX}"
            "${THIRD_PREFIX}/include" "${THIRD_PREFIX}/lib")
    endif ()
endif ()

