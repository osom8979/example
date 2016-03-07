## Find the Google-glog library.
#
# The following variables are optionally searched for defaults
#  GLOG_ROOT_DIR
#
# The following are set after configuration is done:
#  GLOG_FOUND
#  GLOG_INCLUDE_DIRS
#  GLOG_LIBRARIES

set (GLOG_INCLUDE_SEARCH_PATHS
    "/usr/include" "/usr/local/include"
    "${GLOG_ROOT_DIR}" "${GLOG_ROOT_DIR}/include"
    "$ENV{GLOG_ROOT_DIR}" "$ENV{GLOG_ROOT_DIR}/include")
set (GLOG_LIBRARY_SEARCH_PATHS
    "/usr/lib" "/usr/local/lib"
    "${GLOG_ROOT_DIR}" "${GLOG_ROOT_DIR}/lib"
    "$ENV{GLOG_ROOT_DIR}" "$ENV{GLOG_ROOT_DIR}/lib")

set (_glog_header_name  "glog/logging.h")
set (_glog_lib_name     "glog")

find_path (GLOG_INCLUDE_DIR
    NAMES ${_glog_header_name}
    PATHS ${GLOG_INCLUDE_SEARCH_PATHS})
find_library (GLOG_LIBRARY
    NAMES ${_glog_lib_name}
    PATHS ${GLOG_LIBRARY_SEARCH_PATHS}
    PATH_SUFFIXES Debug Release)

set (LOOKED_FOR GLOG_INCLUDE_DIR GLOG_LIBRARY)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (GLOG DEFAULT_MSG ${LOOKED_FOR})

if (GLOG_FOUND)
    set (GLOG_INCLUDE_DIRS ${GLOG_INCLUDE_DIR})
    set (GLOG_LIBRARIES ${GLOG_LIBRARY})
    mark_as_advanced (${LOOKED_FOR})
    message (STATUS "Found glog  (include: ${GLOG_INCLUDE_DIR}, library: ${GLOG_LIBRARY})")
endif ()

