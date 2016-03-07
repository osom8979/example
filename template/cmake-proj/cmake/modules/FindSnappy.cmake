## Find the snappy library.
#
# The following variables are optionally searched for defaults
#  SNAPPY_ROOT_DIR
#
# The following are set after configuration is done:
#  SNAPPY_FOUND
#  SNAPPY_INCLUDE_DIRS
#  SNAPPY_LIBRARIES

set (SNAPPY_INCLUDE_SEARCH_PATHS
    "/usr/include" "/usr/local/include"
    "${SNAPPY_ROOT_DIR}" "${SNAPPY_ROOT_DIR}/include"
    "$ENV{SNAPPY_ROOT_DIR}" "$ENV{SNAPPY_ROOT_DIR}/include")
set (SNAPPY_LIBRARY_SEARCH_PATHS
    "/usr/lib" "/usr/local/lib"
    "${SNAPPY_ROOT_DIR}" "${SNAPPY_ROOT_DIR}/lib"
    "$ENV{SNAPPY_ROOT_DIR}" "$ENV{SNAPPY_ROOT_DIR}/lib")

set (_snappy_header_name  "snappy.h")
set (_snappy_lib_name     "snappy")

find_path (SNAPPY_INCLUDE_DIR
    NAMES ${_snappy_header_name}
    PATHS ${SNAPPY_INCLUDE_SEARCH_PATHS})
find_library (SNAPPY_LIBRARY
    NAMES ${_snappy_lib_name}
    PATHS ${SNAPPY_LIBRARY_SEARCH_PATHS}
    PATH_SUFFIXES Debug Release)

set (LOOKED_FOR SNAPPY_INCLUDE_DIR SNAPPY_LIBRARY)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (SNAPPY DEFAULT_MSG ${LOOKED_FOR})

if (SNAPPY_FOUND)
    set (SNAPPY_INCLUDE_DIRS ${SNAPPY_INCLUDE_DIR})
    set (SNAPPY_LIBRARIES ${SNAPPY_LIBRARY})
    mark_as_advanced (${LOOKED_FOR})
    message (STATUS "Found snappy (include: ${SNAPPY_INCLUDE_DIR}, library: ${SNAPPY_LIBRARY})")
endif ()

