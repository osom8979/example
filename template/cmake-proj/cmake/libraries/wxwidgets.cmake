## CMake build script.

if (VERBOSE)
    message (">> Library name: wxWidgets")
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

set (_proj_name     "wxWidgets-3.1.0")
set (_proj_url      "https://codeload.github.com/wxWidgets/wxWidgets/tar.gz/v3.1.0")
set (_working       "${WORKING_DIR}")
set (_prefix        "${INSTALL_PREFIX}")
set (_include_path  "${INCLUDE_PATH}")
set (_library_path  "${LIBRARY_PATH}")
set (_download_file "${_working}/${_proj_name}.tar.gz")
set (_build_dir     "${_working}/${_proj_name}")

# exists library.
set  (_shared_lib_glob "${CMAKE_SHARED_LIBRARY_PREFIX}wx_base*${CMAKE_SHARED_LIBRARY_SUFFIX}")
file (GLOB_RECURSE _find_shared_libs "${_prefix}/lib/${_shared_lib_glob}")
list (LENGTH _find_shared_libs _find_shared_libs_length)

if (${_find_shared_libs_length} GREATER 0)
    list (GET _find_shared_libs 0 _find_shared_lib_first)
    message (">> Exists library: ${_find_shared_lib_first}")
    return ()
endif ()

# download & build & install library.

include (ProcessorCount)
ProcessorCount (_process_count)
math (EXPR _thread_count "${_process_count} * 2")

if (NOT EXISTS "${_download_file}")
    message (">> Download ${_proj_name}")
    execute_process (
        COMMAND curl -o "${_download_file}" "${_proj_url}"
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
    COMMAND ./configure "--prefix=${_prefix}" --enable-shared --disable-static
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

