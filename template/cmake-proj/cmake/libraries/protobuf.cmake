## Library build properties.

if (_library_verbose)
    #message (">> _library_verbose: ${_library_verbose}")
    message  (">> _library_clean: ${_library_clean}")

    message  (">> _library_name: ${_library_name}")
    message  (">> _library_dir_name: ${_library_dir_name}")
    message  (">> _library_url: ${_library_url}")
    message  (">> _library_md5: ${_library_md5}")
    message  (">> _library_temp_dir: ${_library_temp_dir}")
    message  (">> _library_dest_name: ${_library_dest_name}")
    message  (">> _library_work_name: ${_library_work_name}")
    message  (">> _library_prefix: ${_library_prefix}")
    message  (">> _library_already: ${_library_already}")

    message  (">> _library_update: ${_library_update}")
    message  (">> _library_autoconfig: ${_library_autoconfig}")
    message  (">> _library_configure: ${_library_configure}")
    message  (">> _library_build: ${_library_build}")
    message  (">> _library_install: ${_library_install}")
    message  (">> _library_test: ${_library_test}")
endif ()

include (ProcessorCount)
ProcessorCount (_process_count)
math (EXPR _thread_count "${_process_count} * 2")

set (_library_verbose OFF)
set (_library_clean   OFF)

# [WARNING] Don't change this code.
#set (_library_temp_dir)
#set (_library_prefix)

set (_library_name       "protobuf")
set (_library_dir_name   "${_library_name}-3.0.0-beta-2")
set (_library_url        "https://codeload.github.com/google/protobuf/tar.gz/v3.0.0-beta-2")
set (_library_md5        "")
set (_library_dest_name  "${_library_dir_name}.tar.gz")
set (_library_work_name  "${_library_dir_name}")

set (_install_file_name  "${CMAKE_STATIC_LIBRARY_PREFIX}protobuf${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (_library_already    "${_library_prefix}/lib/${_install_file_name}")

set (_library_update)
set (_library_autoconfig  ./autogen.sh)
set (_library_configure   ./configure "--prefix=${_library_prefix}"
                                      --disable-shared
                                      --enable-static
                                      CFLAGS=-fPIC
                                      CXXFLAGS=-fPIC)
set (_library_build       make -j${_thread_count} V=1)
set (_library_install     make install)
set (_library_test)

