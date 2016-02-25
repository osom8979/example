## CMake google-protocol-buffers build script.

#! download & build & install, zlib library.
#
# \param _working      [in] working directory.
# \param _prefix       [in] install prefix directory.
function (install_zlib _working _prefix)

    set (_proj_name     "zlib-1.2.8")
    set (_proj_url      "http://zlib.net/zlib-1.2.8.tar.gz")
    set (_download_file "${_working}/${_proj_name}.tar.gz")
    set (_build_dir     "${_working}/${_proj_name}")

    if (NOT EXISTS "${_download_file}")
        message ("** Download ${_proj_name}")
        execute_process (
            COMMAND curl -o "${_download_file}" "${_proj_url}"
            WORKING_DIRECTORY "${_working}"
        )
    endif ()

    if (NOT EXISTS "${_build_dir}")
        message ("** Extract ${_proj_name}")
        execute_process (
            COMMAND tar vxzf "${_download_file}"
            WORKING_DIRECTORY "${_working}"
        )
    endif ()

    message ("** Configure ${_proj_name}")
    execute_process (
        COMMAND ./configure --prefix=${_prefix}
        WORKING_DIRECTORY "${_build_dir}"
    )

    message ("** Build ${_proj_name}")
    execute_process (
        COMMAND make
        WORKING_DIRECTORY "${_build_dir}"
    )

    message ("** Install ${_proj_name}")
    execute_process (
        COMMAND make install
        WORKING_DIRECTORY "${_build_dir}"
    )

endfunction ()

