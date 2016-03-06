## CMake zlib build script.

if (VERBOSE)
    message (">> Library name: zlib")
    message (">> Working directory: ${WORKING_DIR}")
    message (">> Install prefix: ${INSTALL_PREFIX}")
    message (">> Include path: ${INCLUDE_PATH}")
    message (">> Library path: ${LIBRARY_PATH}")
    message (">> CMAKE_STATIC_LIBRARY_PREFIX: ${CMAKE_STATIC_LIBRARY_PREFIX}")
    message (">> CMAKE_STATIC_LIBRARY_SUFFIX: ${CMAKE_STATIC_LIBRARY_SUFFIX}")
    message (">> CMAKE_EXECUTABLE_SUFFIX: ${CMAKE_EXECUTABLE_SUFFIX}")
endif ()

set (_proj_name     "zlib-1.2.8")
set (_proj_url      "http://zlib.net/zlib-1.2.8.tar.gz")
set (_working       "${WORKING_DIR}")
set (_prefix        "${INSTALL_PREFIX}")
set (_include_path  "${INCLUDE_PATH}")
set (_library_path  "${LIBRARY_PATH}")
set (_download_file "${_working}/${_proj_name}.tar.gz")
set (_build_dir     "${_working}/${_proj_name}")

# exists library.
set (_static_lib "${_prefix}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}z${CMAKE_STATIC_LIBRARY_SUFFIX}")

if (EXISTS ${_static_lib})
    message (">> Exists library: ${_static_lib}")
    return ()
endif ()

# download & build & install library.

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
    COMMAND ./configure --prefix=${_prefix} --static
    WORKING_DIRECTORY "${_build_dir}")

message (">> Build ${_proj_name}")
execute_process (
    COMMAND make CFLAGS=-fPIC
    WORKING_DIRECTORY "${_build_dir}")

message (">> Install ${_proj_name}")
execute_process (
    COMMAND make install
    WORKING_DIRECTORY "${_build_dir}")

message (">> Done.")

