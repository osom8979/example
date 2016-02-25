## CMake google-protocol-buffers build script.

#! download & build & install, google protobuf library.
#
# \param _working      [in] working directory.
# \param _prefix       [in] install prefix directory.
# \param _include_path [in] CPP, include path.
# \param _library_path [in] LINK, library path.
function (install_protobuf _working _prefix _include_path _library_path)

    set (_proj_name "protobuf")
    set (_proj_url  "https://github.com/google/protobuf.git")
    set (_build_dir "${_working}/${_proj_name}")

    if (NOT EXISTS "${_build_dir}")
        message ("** Download ${_proj_name}")
        execute_process (
            COMMAND git clone --depth=1 ${_proj_url}
            WORKING_DIRECTORY "${_working}"
        )
    endif ()

    if (NOT EXISTS "${_build_dir}/configure")
        message ("** Autogen ${_proj_name}")
        execute_process (
            COMMAND ./autogen.sh
            WORKING_DIRECTORY "${_build_dir}"
        )
    endif ()

    message ("** Configure ${_proj_name}")
    execute_process (
        COMMAND ./configure --disable-shared --enable-static
                            --prefix=${_prefix}
                            #CPPFLAGS=-I${_include_path}
                            #LDFLAGS=-L${_library_path}
        WORKING_DIRECTORY "${_build_dir}"
    )

    include (ProcessorCount)
    ProcessorCount (_process_count)
    math (EXPR _thread_count "${_process_count} * 2")

    message ("** Build ${_proj_name}")
    execute_process (
        COMMAND make -j${_thread_count} V=0
        WORKING_DIRECTORY "${_build_dir}"
    )

    message ("** Install ${_proj_name}")
    execute_process (
        COMMAND make install
        WORKING_DIRECTORY "${_build_dir}"
    )
endfunction ()

