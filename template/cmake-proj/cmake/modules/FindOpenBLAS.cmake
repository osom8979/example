## Find the OpenBLAS library.
#
# The following variables are optionally searched for defaults
#  OPENBLAS_ROOT_DIR
#
# The following are set after configuration is done:
#  OPENBLAS_FOUND
#  OPENBLAS_INCLUDE_DIRS
#  OPENBLAS_LIBRARIES

set (OPENBLAS_INCLUDE_SEARCH_PATHS
    "/usr/include" "/usr/local/include"
    "${OPENBLAS_ROOT_DIR}" "${OPENBLAS_ROOT_DIR}/include"
    "$ENV{OPENBLAS_ROOT_DIR}" "$ENV{OPENBLAS_ROOT_DIR}/include")
set (OPENBLAS_LIBRARY_SEARCH_PATHS
    "/usr/lib" "/usr/local/lib"
    "${OPENBLAS_ROOT_DIR}" "${OPENBLAS_ROOT_DIR}/lib"
    "$ENV{OPENBLAS_ROOT_DIR}" "$ENV{OPENBLAS_ROOT_DIR}/lib")

set (_openblas_header_name  "openblas_config.h")
set (_openblas_lib_name     "openblas")

find_path (OPENBLAS_INCLUDE_DIR
    NAMES ${_openblas_header_name}
    PATHS ${OPENBLAS_INCLUDE_SEARCH_PATHS})
find_library (OPENBLAS_LIBRARY
    NAMES ${_openblas_lib_name}
    PATHS ${OPENBLAS_LIBRARY_SEARCH_PATHS}
    PATH_SUFFIXES Debug Release)

set (LOOKED_FOR OPENBLAS_INCLUDE_DIR OPENBLAS_LIBRARY)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (OPENBLAS DEFAULT_MSG ${LOOKED_FOR})

if (OPENBLAS_FOUND)
    set (OPENBLAS_INCLUDE_DIRS ${OPENBLAS_INCLUDE_DIR})
    set (OPENBLAS_LIBRARIES ${OPENBLAS_LIBRARY})
    mark_as_advanced (${LOOKED_FOR})
    message (STATUS "Found OpenBLAS (include: ${OPENBLAS_INCLUDE_DIR}, library: ${OPENBLAS_LIBRARY})")
endif ()

