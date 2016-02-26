## CMake third build script.

exists_define_or_die (THIRD_PREFIX)

set (_temp_prefix     "${THIRD_PREFIX}/temp")
set (_binary_prefix   "${THIRD_PREFIX}/bin")
set (_include_prefix  "${THIRD_PREFIX}/include")
set (_library_prefix  "${THIRD_PREFIX}/lib")

# Build directory.
if (NOT EXISTS ${_temp_prefix})
    file (MAKE_DIRECTORY ${_temp_prefix})
endif ()

if (USE_ZLIB)
    message ("** Use the zlib.")

    set (_zlib_static_lib "${_library_prefix}/libz.a")

    if (EXISTS ${_zlib_static_lib})
        message ("** Exists zlib library: ${_zlib_static_lib}")
    else ()
        message ("** Install zlib library.")
        install_zlib ("${_temp_prefix}" "${THIRD_PREFIX}")
    endif ()
endif ()

if (USE_PROTOBUF)
    message ("** Use the google-protocol-buffers.")

    set (_protobuf_static_lib "${_library_prefix}/libprotobuf.a")
    set (_protobuf_compiler   "${_binary_prefix}/protoc")

    if (EXISTS ${_protobuf_static_lib} AND EXISTS ${_protobuf_compiler})
        message ("** Exists protobuf library: ${_protobuf_static_lib}")
    else ()
        message ("** Install protobuf library.")
        install_protobuf ("${_temp_prefix}" "${THIRD_PREFIX}"
            "${_include_prefix}" "${_library_prefix}")
    endif ()
endif ()

