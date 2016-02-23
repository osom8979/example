## CMake sample  build script.

if (MAIN_NAME STREQUAL "")
    message (FATAL_ERROR "Not found MAIN_NAME variable.")
endif()

## -----------------------------
## Enter the name of the sample.
set (SAMPLE_NAME "sample")
## -----------------------------

project (${SAMPLE_NAME} C CXX)

set (${SAMPLE_NAME}_VERSION_MAJOR  ${MAIN_NAME}_VERSION_MAJOR)
set (${SAMPLE_NAME}_VERSION_MINOR  ${MAIN_NAME}_VERSION_MINOR)
set (${SAMPLE_NAME}_VERSION_PATCH  ${MAIN_NAME}_VERSION_PATCH)
set (${SAMPLE_NAME}_VERSION_TWEAK  ${MAIN_NAME}_VERSION_TWEAK)

set (SAMPLE_SRC_DIR     ".")
set (SAMPLE_SRC_SUFFIX  "*.cpp")

file (GLOB_RECURSE SAMPLE_SRCS ${SAMPLE_SRC_DIR} ${SAMPLE_SRC_SUFFIX})

add_executable (${SAMPLE_NAME} ${SAMPLE_SRCS})
add_dependencies (${SAMPLE_NAME} ${MAIN_NAME})
target_link_libraries (${SAMPLE_NAME} ${MAIN_NAME})

