## CMake build script.

if (VERBOSE)
    message (">> Library name: leveldb")
    message (">> Working directory: ${WORKING_DIR}")
    message (">> Install prefix: ${INSTALL_PREFIX}")
    message (">> Include path: ${INCLUDE_PATH}")
    message (">> Library path: ${LIBRARY_PATH}")
    message (">> CMAKE_STATIC_LIBRARY_PREFIX: ${CMAKE_STATIC_LIBRARY_PREFIX}")
    message (">> CMAKE_STATIC_LIBRARY_SUFFIX: ${CMAKE_STATIC_LIBRARY_SUFFIX}")
    message (">> CMAKE_EXECUTABLE_SUFFIX: ${CMAKE_EXECUTABLE_SUFFIX}")
endif ()

set (_proj_name     "leveldb-1.18")
set (_proj_url      "https://codeload.github.com/google/leveldb/tar.gz/v1.18")
set (_working       "${WORKING_DIR}")
set (_prefix        "${INSTALL_PREFIX}")
set (_include_path  "${INCLUDE_PATH}")
set (_library_path  "${LIBRARY_PATH}")
set (_download_file "${_working}/${_proj_name}.tar.gz")
set (_build_dir     "${_working}/${_proj_name}")

# exists library.
set (_static_lib_name  "${CMAKE_STATIC_LIBRARY_PREFIX}leveldb${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (_static_lib       "${_prefix}/lib/${_static_lib_name}")

if (EXISTS ${_static_lib})
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
        COMMAND curl -o "${_download_file}" "${_proj_url}"
        WORKING_DIRECTORY "${_working}")
endif ()

if (NOT EXISTS "${_build_dir}")
    message (">> Extract ${_proj_name}")
    execute_process (
        COMMAND tar xzf "${_download_file}"
        WORKING_DIRECTORY "${_working}")
endif ()

message (">> Build ${_proj_name}")
execute_process (
    COMMAND make -j${_thread_count}
    WORKING_DIRECTORY "${_build_dir}")

message (">> Install ${_proj_name}")

#set  (_library_prefix "${_prefix}/lib")
#file (GLOB_RECURSE _find_install_libs "${_build_dir}/${CMAKE_STATIC_LIBRARY_PREFIX}leveldb*")
#foreach (_cursor ${_find_install_libs})
#    string (REPLACE "${_build_dir}" "${_library_prefix}" _install_destination "${_cursor}")
#    execute_process (COMMAND ${CMAKE_COMMAND} -E copy "${_cursor}" "${_install_destination}")
#endforeach ()

execute_process (COMMAND ${CMAKE_COMMAND} -E copy "${_build_dir}/${_static_lib_name}"  "${_static_lib}")
execute_process (COMMAND ${CMAKE_COMMAND} -E copy_directory "${_build_dir}/include" "${_prefix}/include")

message (">> Done.")

