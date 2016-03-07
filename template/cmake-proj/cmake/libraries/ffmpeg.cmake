## CMake build script.

if (VERBOSE)
    message (">> Library name: ffmpeg")
    message (">> Working directory: ${WORKING_DIR}")
    message (">> Install prefix: ${INSTALL_PREFIX}")
    message (">> Include path: ${INCLUDE_PATH}")
    message (">> Library path: ${LIBRARY_PATH}")
    message (">> CMAKE_STATIC_LIBRARY_PREFIX: ${CMAKE_STATIC_LIBRARY_PREFIX}")
    message (">> CMAKE_STATIC_LIBRARY_SUFFIX: ${CMAKE_STATIC_LIBRARY_SUFFIX}")
    message (">> CMAKE_SHARED_LIBRARY_PREFIX: ${CMAKE_SHARED_LIBRARY_PREFIX}")
    message (">> CMAKE_SHARED_LIBRARY_SUFFIX: ${CMAKE_SHARED_LIBRARY_SUFFIX}")
    message (">> CMAKE_EXECUTABLE_SUFFIX: ${CMAKE_EXECUTABLE_SUFFIX}")
endif ()

set (_proj_name     "ffmpeg-3.0")
set (_proj_url      "https://www.ffmpeg.org/releases/ffmpeg-3.0.tar.gz")
set (_working       "${WORKING_DIR}")
set (_prefix        "${INSTALL_PREFIX}")
set (_include_path  "${INCLUDE_PATH}")
set (_library_path  "${LIBRARY_PATH}")
set (_download_file "${_working}/${_proj_name}.tar.gz")
set (_build_dir     "${_working}/${_proj_name}")

# exists library.
set (_shared_lib_name  "${CMAKE_SHARED_LIBRARY_PREFIX}avcodec${CMAKE_SHARED_LIBRARY_SUFFIX}")
set (_shared_lib       "${_prefix}/lib/${_shared_lib_name}")

if (EXISTS ${_shared_lib})
    message (">> Exists library: ${_shared_lib}")
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

message (">> Configure ${_proj_name}")
execute_process (
    COMMAND ./configure "--prefix=${_prefix}"
                        --enable-shared --disable-static
                        --disable-yasm
                        --extra-cflags="-fPIC"
    WORKING_DIRECTORY "${_build_dir}")

message (">> Build ${_proj_name}")
execute_process (
    COMMAND make -j${_thread_count}
    WORKING_DIRECTORY "${_build_dir}")

message (">> Install ${_proj_name}")
execute_process (
    COMMAND make install
    WORKING_DIRECTORY "${_build_dir}")

message (">> Done.")
