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

set (_library_verbose OFF)
set (_library_clean   OFF)

# [WARNING] Don't change this code.
#set (_library_temp_dir)
#set (_library_prefix)

set (_library_name       "zlib")
set (_library_dir_name   "${_library_name}-1.2.8")
set (_library_url        "http://zlib.net/zlib-1.2.8.tar.gz")
set (_library_md5        "44d667c142d7cda120332623eab69f40")
set (_library_dest_name  "${_library_dir_name}.tar.gz")
set (_library_work_name  "${_library_dir_name}")

set (_install_file_name  "${CMAKE_STATIC_LIBRARY_PREFIX}z${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (_library_already    "${_library_prefix}/lib/${_install_file_name}")

set (_library_update)
set (_library_autoconfig)
set (_library_configure   ./configure "--prefix=${_library_prefix}" --static)
set (_library_build       make CFLAGS=-fPIC)
set (_library_install     make install)
set (_library_test)

