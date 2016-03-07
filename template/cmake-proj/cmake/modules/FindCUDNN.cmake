## Find the cuDNN library.
#
# The following variables are optionally searched for defaults
#  CUDNN_ROOT_DIR
#
# The following are set after configuration is done:
#  CUDNN_FOUND
#  CUDNN_INCLUDE_DIRS
#  CUDNN_LIBRARIES

if (NOT CUDA_FOUND)
    return ()
endif ()

set (CUDNN_INCLUDE_SEARCH_PATHS
    "/usr/include" "/usr/local/include"
    "/usr/cuda/include" "/usr/local/cuda/include"
    "${CUDNN_ROOT_DIR}" "${CUDNN_ROOT_DIR}/include"
    "$ENV{CUDNN_ROOT_DIR}" "$ENV{CUDNN_ROOT_DIR}/include")
set (CUDNN_LIBRARY_SEARCH_PATHS
    "/usr/lib" "/usr/local/lib"
    "/usr/cuda/lib" "/usr/local/cuda/lib"
    "/usr/cuda/lib64" "/usr/local/cuda/lib64"
    "${CUDNN_ROOT_DIR}" "${CUDNN_ROOT_DIR}/lib" "${CUDNN_ROOT_DIR}/lib64"
    "$ENV{CUDNN_ROOT_DIR}" "$ENV{CUDNN_ROOT_DIR}/lib" "$ENV{CUDNN_ROOT_DIR}/lib64")

set (_cudnn_header_name  "cudnn.h")
set (_cudnn_lib_name     "cudnn")

find_path (CUDNN_INCLUDE_DIR
    NAMES ${_cudnn_header_name}
    PATHS ${CUDNN_INCLUDE_SEARCH_PATHS})
find_library (CUDNN_LIBRARY
    NAMES ${_cudnn_lib_name}
    PATHS ${CUDNN_LIBRARY_SEARCH_PATHS}
    PATH_SUFFIXES Debug Release)

set (LOOKED_FOR CUDNN_INCLUDE_DIR CUDNN_LIBRARY)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (CUDNN DEFAULT_MSG ${LOOKED_FOR})

if (CUDNN_FOUND)
    set (CUDNN_INCLUDE_DIRS ${CUDNN_INCLUDE_DIR})
    set (CUDNN_LIBRARIES ${CUDNN_LIBRARY})
    mark_as_advanced (${LOOKED_FOR})
    message (STATUS "Found cuDNN (include: ${CUDNN_INCLUDE_DIR}, library: ${CUDNN_LIBRARY})")
endif ()

