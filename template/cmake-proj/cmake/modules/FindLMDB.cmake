## Find the LMDB library.
#
# The following variables are optionally searched for defaults
#  LMDB_ROOT_DIR
#
# The following are set after configuration is done:
#  LMDB_FOUND
#  LMDB_INCLUDE_DIRS
#  LMDB_LIBRARIES

set (LMDB_INCLUDE_SEARCH_PATHS
    "/usr/include" "/usr/local/include"
    "${LMDB_ROOT_DIR}" "${LMDB_ROOT_DIR}/include"
    "$ENV{LMDB_ROOT_DIR}" "$ENV{LMDB_ROOT_DIR}/include")
set (LMDB_LIBRARY_SEARCH_PATHS
    "/usr/lib" "/usr/local/lib"
    "${LMDB_ROOT_DIR}" "${LMDB_ROOT_DIR}/lib"
    "$ENV{LMDB_ROOT_DIR}" "$ENV{LMDB_ROOT_DIR}/lib")

set (_lmdb_header_name  "lmdb.h")
set (_lmdb_lib_name     "lmdb")

find_path (LMDB_INCLUDE_DIR
    NAMES ${_lmdb_header_name}
    PATHS ${LMDB_INCLUDE_SEARCH_PATHS})
find_library (LMDB_LIBRARY
    NAMES ${_lmdb_lib_name}
    PATHS ${LMDB_LIBRARY_SEARCH_PATHS}
    PATH_SUFFIXES Debug Release)

set (LOOKED_FOR LMDB_INCLUDE_DIR LMDB_LIBRARY)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (LMDB DEFAULT_MSG ${LOOKED_FOR})

if (LMDB_FOUND)
    set (LMDB_INCLUDE_DIRS ${LMDB_INCLUDE_DIR})
    set (LMDB_LIBRARIES ${LMDB_LIBRARY})
    mark_as_advanced (${LOOKED_FOR})
    message (STATUS "Found lmdb (include: ${LMDB_INCLUDE_DIR}, library: ${LMDB_LIBRARY})")
endif ()

