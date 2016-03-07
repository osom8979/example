## Find the libuv library.
#
# The following variables are optionally searched for defaults
#  UV_ROOT_DIR
#
# The following are set after configuration is done:
#  UV_FOUND
#  UV_INCLUDE_DIRS
#  UV_LIBRARIES

set (UV_INCLUDE_SEARCH_PATHS
    "/usr/include" "/usr/local/include"
    "${UV_ROOT_DIR}" "${UV_ROOT_DIR}/include"
    "$ENV{UV_ROOT_DIR}" "$ENV{UV_ROOT_DIR}/include")
set (UV_LIBRARY_SEARCH_PATHS
    "/usr/lib" "/usr/local/lib"
    "${UV_ROOT_DIR}" "${UV_ROOT_DIR}/lib"
    "$ENV{UV_ROOT_DIR}" "$ENV{UV_ROOT_DIR}/lib")

set (_uv_header_name  "uv.h")
set (_uv_lib_name     "uv")

find_path (UV_INCLUDE_DIR
    NAMES ${_uv_header_name}
    PATHS ${UV_INCLUDE_SEARCH_PATHS})
find_library (UV_LIBRARY
    NAMES ${_uv_lib_name}
    PATHS ${UV_LIBRARY_SEARCH_PATHS}
    PATH_SUFFIXES Debug Release)

set (LOOKED_FOR UV_INCLUDE_DIR UV_LIBRARY)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (UV DEFAULT_MSG ${LOOKED_FOR})

if (UV_FOUND)
    set (UV_INCLUDE_DIRS ${UV_INCLUDE_DIR})
    set (UV_LIBRARIES ${UV_LIBRARY})
    mark_as_advanced (${LOOKED_FOR})
    message (STATUS "Found libuv (include: ${UV_INCLUDE_DIR}, library: ${UV_LIBRARY})")
endif ()

