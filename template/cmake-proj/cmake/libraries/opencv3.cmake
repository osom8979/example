## CMake OpenCV 3.x build script.

if (VERBOSE)
    message (">> Library name: opencv 3.x")
    message (">> Working directory: ${WORKING_DIR}")
    message (">> Install prefix: ${INSTALL_PREFIX}")
    message (">> Include path: ${INCLUDE_PATH}")
    message (">> Library path: ${LIBRARY_PATH}")
    message (">> CMAKE_STATIC_LIBRARY_PREFIX: ${CMAKE_STATIC_LIBRARY_PREFIX}")
    message (">> CMAKE_STATIC_LIBRARY_SUFFIX: ${CMAKE_STATIC_LIBRARY_SUFFIX}")
    message (">> CMAKE_EXECUTABLE_SUFFIX: ${CMAKE_EXECUTABLE_SUFFIX}")
endif ()

set (_proj_name     "opencv-3.1.0")
set (_proj_url      "https://codeload.github.com/Itseez/opencv/tar.gz/3.1.0")
set (_working       "${WORKING_DIR}")
set (_prefix        "${INSTALL_PREFIX}")
set (_include_path  "${INCLUDE_PATH}")
set (_library_path  "${LIBRARY_PATH}")
set (_download_file "${_working}/${_proj_name}.tar.gz")
set (_build_dir     "${_working}/${_proj_name}")

# exists library.
set (_static_lib_name  "${CMAKE_STATIC_LIBRARY_PREFIX}opencv_ts${CMAKE_STATIC_LIBRARY_SUFFIX}")
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
        COMMAND curl -k -o "${_download_file}" "${_proj_url}"
        WORKING_DIRECTORY "${_working}")
endif ()

if (NOT EXISTS "${_build_dir}")
    message (">> Extract ${_proj_name}")
    execute_process (
        COMMAND tar xzf "${_download_file}"
        WORKING_DIRECTORY "${_working}")
endif ()

if (NOT EXISTS "${_build_dir}/Makefile")
    message (">> Configure ${_proj_name}")
    execute_process (
    COMMAND ${CMAKE_COMMAND} -DBUILD_SHARED_LIBS=OFF
                             -DCMAKE_BUILD_TYPE=Release
                             -DCMAKE_INSTALL_PREFIX=${_prefix}
                             -DWITH_CUDA=OFF
                             -G "Unix Makefiles"
    WORKING_DIRECTORY "${_build_dir}")
endif ()

message (">> Build ${_proj_name}")
execute_process (
    COMMAND make -j${_thread_count}
    WORKING_DIRECTORY "${_build_dir}")

message (">> Install ${_proj_name}")
execute_process (
    COMMAND make install
    WORKING_DIRECTORY "${_build_dir}")

message (">> Done.")

