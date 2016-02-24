## CMake google-protocol-buffers build script.

#! download & build & install, zlib library.
#
# \param _working      [in] working directory.
# \param _prefix       [in] install prefix directory.
# \param _include_path [in] CPP, include path.
# \param _library_parh [in] LINK, library path.
function (install_zlib _working _prefix _include_path _library_path)

    set (_proj_name     "zlib-1.2.8")
    set (_proj_url      "http://zlib.net/zlib-1.2.8.tar.gz")
    set (_parent_dir    "${_working}")
    set (_download_file "${_parent_dir}/${_proj_name}.tar.gz")
    set (_build_dir     "${_parent_dir}/${_proj_name}")
    set (_prefix_dir    "${_prefix}")
    set (_include_dir   "${_include_path}")
    set (_library_dir   "${_library_path}")

    # Download & extract.
    execute_process (
        COMMAND curl -o "${_download_file}" "${_proj_url}"
        WORKING_DIRECTORY "${_parent_dir}"
    )
    execute_process (
        COMMAND tar vxzf "${_download_file}"
        WORKING_DIRECTORY "${_parent_dir}"
    )
    execute_process (
        COMMAND ./configure --prefix=${_prefix_dir}
        WORKING_DIRECTORY "${_build_dir}"
    )
    execute_process (
        COMMAND make
        WORKING_DIRECTORY "${_build_dir}"
    )
    execute_process (
        COMMAND make install
        WORKING_DIRECTORY "${_build_dir}"
    )

endfunction ()

