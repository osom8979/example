## CMake google-protocol-buffers build script.

#! download & build & install, zlib library.
#
# \param working      [in] working directory.
# \param prefix       [in] install prefix directory.
# \param include_path [in] CPP, include path.
# \param library_parh [in] LINK, library path.
function (install_zlib working prefix include_path library_path)
    set (WORKING_DIR  "${working}")
    set (ZLIB_FILE    "${WORKING_DIR}/zlib-1.2.8.tar.gz")
    set (ZLIB_DIR     "${WORKING_DIR}/zlib-1.2.8")
    set (PREFIX_DIR   "${prefix}")
    set (INCLUDE_DIR  "${include_path}")
    set (LIBRARY_DIR  "${library_path}")

    # Download.
    execute_process (
        COMMAND curl -o "${ZLIB_FILE}" http://zlib.net/zlib-1.2.8.tar.gz
        WORKING_DIRECTORY "${WORKING_DIR}"
    )

    # Extract.
    execute_process (
        COMMAND tar vxzf "${ZLIB_FILE}"
        WORKING_DIRECTORY "${WORKING_DIR}"
    )

    # Run configure.
    execute_process (
        COMMAND ./configure --prefix=${PREFIX_DIR}
        WORKING_DIRECTORY "${ZLIB_DIR}"
    )

    # Build.
    execute_process (
        COMMAND make
        WORKING_DIRECTORY "${ZLIB_DIR}"
    )

    # Install
    execute_process (
        COMMAND make install
        WORKING_DIRECTORY "${ZLIB_DIR}"
    )
endfunction ()

