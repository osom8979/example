## Find the LevelDB library.
#
# The following variables are optionally searched for defaults
#  LEVELDB_ROOT_DIR
#
# The following are set after configuration is done:
#  LEVELDB_FOUND
#  LEVELDB_INCLUDE_DIRS
#  LEVELDB_LIBRARIES

set (LEVELDB_INCLUDE_SEARCH_PATHS
    "/usr/include" "/usr/local/include"
    "${LEVELDB_ROOT_DIR}" "${LEVELDB_ROOT_DIR}/include"
    "$ENV{LEVELDB_ROOT_DIR}" "$ENV{LEVELDB_ROOT_DIR}/include")
set (LEVELDB_LIBRARY_SEARCH_PATHS
    "/usr/lib" "/usr/local/lib"
    "${LEVELDB_ROOT_DIR}" "${LEVELDB_ROOT_DIR}/lib"
    "$ENV{LEVELDB_ROOT_DIR}" "$ENV{LEVELDB_ROOT_DIR}/lib")

set (_leveldb_header_name  "leveldb/options.h")
set (_leveldb_lib_name     "leveldb")

find_path (LEVELDB_INCLUDE_DIR
    NAMES ${_leveldb_header_name}
    PATHS ${LEVELDB_INCLUDE_SEARCH_PATHS})
find_library (LEVELDB_LIBRARY
    NAMES ${_leveldb_lib_name}
    PATHS ${LEVELDB_LIBRARY_SEARCH_PATHS}
    PATH_SUFFIXES Debug Release)

set (LOOKED_FOR LEVELDB_INCLUDE_DIR LEVELDB_LIBRARY)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (LEVELDB DEFAULT_MSG ${LOOKED_FOR})

if (LEVELDB_FOUND)
    set (LEVELDB_INCLUDE_DIRS ${LEVELDB_INCLUDE_DIR})
    set (LEVELDB_LIBRARIES ${LEVELDB_LIBRARY})
    mark_as_advanced (${LOOKED_FOR})
    message (STATUS "Found leveldb (include: ${LEVELDB_INCLUDE_DIR}, library: ${LEVELDB_LIBRARY})")
endif ()

