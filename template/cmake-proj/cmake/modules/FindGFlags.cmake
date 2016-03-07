## Find the Google-gflags program.
#
# The following variables are optionally searched for defaults
#  GFLAGS_ROOT_DIR
#
# The following are set after configuration is done:
#  GFLAGS_FOUND
#  GFLAGS_INCLUDE_DIRS
#  GFLAGS_LIBRARIES

set (GFLAGS_INCLUDE_SEARCH_PATHS
    "/usr/include" "/usr/local/include"
    "${GFLAGS_ROOT_DIR}" "${GFLAGS_ROOT_DIR}/include"
    "$ENV{GFLAGS_ROOT_DIR}" "$ENV{GFLAGS_ROOT_DIR}/include")
set (GFLAGS_LIBRARY_SEARCH_PATHS
    "/usr/lib" "/usr/local/lib"
    "${GFLAGS_ROOT_DIR}" "${GFLAGS_ROOT_DIR}/lib"
    "$ENV{GFLAGS_ROOT_DIR}" "$ENV{GFLAGS_ROOT_DIR}/lib")

set (_gflags_header_name  "gflags/gflags.h")
set (_gflags_lib_name     "gflags")

find_path (GFLAGS_INCLUDE_DIR
    NAMES ${_gflags_header_name}
    PATHS ${GFLAGS_INCLUDE_SEARCH_PATHS})
find_library (GFLAGS_LIBRARY
    NAMES ${_gflags_lib_name}
    PATHS ${GFLAGS_LIBRARY_SEARCH_PATHS}
    PATH_SUFFIXES Debug Release)


set (LOOKED_FOR GFLAGS_INCLUDE_DIR GFLAGS_LIBRARY)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (GFLAGS DEFAULT_MSG ${LOOKED_FOR})

if (GFLAGS_FOUND)
    set (GFLAGS_INCLUDE_DIRS ${GFLAGS_INCLUDE_DIR})
    set (GFLAGS_LIBRARIES ${GFLAGS_LIBRARY})
    mark_as_advanced (${LOOKED_FOR})
    message (STATUS "Found gflags  (include: ${GFLAGS_INCLUDE_DIR}, library: ${GFLAGS_LIBRARY})")
endif ()

