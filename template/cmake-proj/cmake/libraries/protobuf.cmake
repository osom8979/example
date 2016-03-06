## CMake google-protocol-buffers build script.

if (VERBOSE)
    message (">> Library name: google-protocol-buffers")
    message (">> Working directory: ${WORKING_DIR}")
    message (">> Install prefix: ${INSTALL_PREFIX}")
    message (">> Include path: ${INCLUDE_PATH}")
    message (">> Library path: ${LIBRARY_PATH}")
    message (">> CMAKE_STATIC_LIBRARY_PREFIX: ${CMAKE_STATIC_LIBRARY_PREFIX}")
    message (">> CMAKE_STATIC_LIBRARY_SUFFIX: ${CMAKE_STATIC_LIBRARY_SUFFIX}")
    message (">> CMAKE_EXECUTABLE_SUFFIX: ${CMAKE_EXECUTABLE_SUFFIX}")
endif ()


set (_proj_name     "protobuf-3.0.0-beta-2")
set (_proj_url      "https://codeload.github.com/google/protobuf/tar.gz/v3.0.0-beta-2")
set (_working       "${WORKING_DIR}")
set (_prefix        "${INSTALL_PREFIX}")
set (_include_path  "${INCLUDE_PATH}")
set (_library_path  "${LIBRARY_PATH}")
set (_download_file "${_working}/${_proj_name}.tar.gz")
set (_build_dir     "${_working}/${_proj_name}")

# exists library.
set (_compiler   "${_prefix}/bin/protoc${CMAKE_EXECUTABLE_SUFFIX}")
set (_static_lib "${_prefix}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}protobuf${CMAKE_STATIC_LIBRARY_SUFFIX}")

if (EXISTS ${_compiler} AND EXISTS ${_static_lib})
    message (">> Exists binary: ${_compiler}")
    message (">> Exists library: ${_static_lib}")
    return ()
endif ()

# download & build & install library.

include (ProcessorCount)
ProcessorCount (_process_count)
math (EXPR _thread_count "${_process_count} * 2")

if (NOT EXISTS "${_download_file}")
    message (">> Download ${_proj_name}")
    execute_process (
        COMMAND curl -k -o "${_download_file}" "${_proj_url}"
        WORKING_DIRECTORY "${_working}")
endif ()

if (NOT EXISTS "${_build_dir}")
    message (">> Extract ${_proj_name}")
    execute_process (
        COMMAND tar xzf "${_download_file}"
        WORKING_DIRECTORY "${_working}")
endif ()

if (NOT EXISTS "${_build_dir}/configure")
    message (">> Autogen ${_proj_name}")
    execute_process (
        COMMAND ./autogen.sh
        WORKING_DIRECTORY "${_build_dir}")
endif ()

if (NOT EXISTS "${_build_dir}/Makefile")
    message (">> Configure ${_proj_name}")
    execute_process (
        COMMAND ./configure --disable-shared --enable-static --quiet
                            --prefix=${_prefix}
                            CFLAGS=-fPIC
                            CXXFLAGS=-fPIC
                            #"LDFLAGS=-Wl,-rpath,${_library_path}"
                            #LT_SYS_LIBRARY_PATH=${_library_path}
                            #CPPFLAGS=-I${_include_path}
                            #LDFLAGS=-L${_library_path}
        WORKING_DIRECTORY "${_build_dir}")
endif ()

message (">> Build ${_proj_name}")
execute_process (
    COMMAND make -j${_thread_count} V=0
    WORKING_DIRECTORY "${_build_dir}")

message (">> Install ${_proj_name}")
execute_process (
    COMMAND make install
    WORKING_DIRECTORY "${_build_dir}")

message (">> Done.")

