## Library build properties.

if (_library_verbose)
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

# [WARNING] Don't change this code.
#set (_library_temp_dir)
#set (_library_prefix)

set (_library_name       "wxWidgets")
set (_library_dir_name   "${_library_name}-3.1.0")
set (_library_url        "https://codeload.github.com/wxWidgets/wxWidgets/tar.gz/v3.1.0")
set (_library_md5        "6d2af648c5d0b2d366e7050d06b9a89f")
set (_library_dest_name  "${_library_dir_name}.tar.gz")
set (_library_work_name  "${_library_dir_name}")

set (_library_already    "${_library_prefix}/include/wx-3.1/wx/config.h")

set (_library_update)
set (_library_autoconfig)
set (_library_configure   ./configure "--prefix=${_library_prefix}" --enable-shared --disable-static)
set (_library_build       make -j${_thread_count})
set (_library_install     make install)
set (_library_test)

