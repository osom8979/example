## CMake google-protocol-buffers build script.

#! download & build & install, google protobuf library.
#
# \param _working      [in] working directory.
# \param _prefix       [in] install prefix directory.
# \param _include_path [in] CPP, include path.
# \param _library_parh [in] LINK, library path.
function (install_protobuf _working _prefix _include_path _library_path)

    set (_proj_name    "protobuf")
    set (_proj_url     "https://github.com/google/protobuf.git")
    set (_parent_dir   "${_working}")
    set (_build_dir    "${_parent_dir}/${_proj_name}")
    set (_prefix_dir   "${_prefix}")
    set (_include_dir  "${_include_path}")
    set (_library_dir  "${_library_path}")

    # Download.
    execute_process (
        COMMAND git clone --depth=1 ${_proj_url}
        WORKING_DIRECTORY "${_parent_dir}"
    )

    # Build & install.
    execute_process (
        COMMAND ./autogen.sh
        WORKING_DIRECTORY "${_build_dir}"
    )
    execute_process (
        COMMAND ./configure --prefix=${_prefix_dir} CPPFLAGS=-I${_include_dir} LDFLAGS=-L${_library_dir}
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

