## CMake google-protocol-buffers build script.

#! download & build & install, google protobuf library.
#
# \param working      [in] working directory.
# \param prefix       [in] install prefix directory.
# \param include_path [in] CPP, include path.
# \param library_parh [in] LINK, library path.
function (install_protobuf working prefix include_path library_path)
    set (WORKING_DIR   "${working}")
    set (PROTOBUF_DIR  "${WORKING_DIR}/protobuf")
    set (PREFIX_DIR    "${prefix}")
    set (INCLUDE_DIR   "${include_path}")
    set (LIBRARY_DIR   "${library_path}")

    # Download.
    execute_process (
        COMMAND git clone --depth=1 https://github.com/google/protobuf.git
        WORKING_DIRECTORY "${WORKING_DIR}"
    )

    # Generate configure.
    execute_process (
        COMMAND ./autogen.sh
        WORKING_DIRECTORY "${PROTOBUF_DIR}"
    )

    # Run configure.
    execute_process (
        COMMAND ./configure --prefix=${PREFIX_DIR} CPPFLAGS=-I${INCLUDE_DIR} LDFLAGS=-L${LIBRARY_DIR}
        WORKING_DIRECTORY "${PROTOBUF_DIR}"
    )

    # Build.
    execute_process (
        COMMAND make
        WORKING_DIRECTORY "${PROTOBUF_DIR}"
    )

    # Install
    execute_process (
        COMMAND make install
        WORKING_DIRECTORY "${PROTOBUF_DIR}"
    )
endfunction ()

