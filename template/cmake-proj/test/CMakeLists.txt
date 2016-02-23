## CMake test  build script.

if (MAIN_NAME STREQUAL "")
    message (FATAL_ERROR "Not found MAIN_NAME variable.")
endif()

## ---------------------------
## Enter the name of the test.
set (TEST_NAME "tester")
## ---------------------------

project (${TEST_NAME} C CXX)

set (${TEST_NAME}_VERSION_MAJOR  ${MAIN_NAME}_VERSION_MAJOR)
set (${TEST_NAME}_VERSION_MINOR  ${MAIN_NAME}_VERSION_MINOR)
set (${TEST_NAME}_VERSION_PATCH  ${MAIN_NAME}_VERSION_PATCH)
set (${TEST_NAME}_VERSION_TWEAK  ${MAIN_NAME}_VERSION_TWEAK)

set (TEST_SRC_DIR     ".")
set (TEST_SRC_SUFFIX  "*.cpp")

file (GLOB_RECURSE TEST_SRCS ${TEST_SRC_DIR} ${TEST_SRC_SUFFIX})

add_executable (${TEST_NAME} ${TEST_SRCS})
add_dependencies (${TEST_NAME} ${MAIN_NAME})
target_link_libraries (${TEST_NAME} ${MAIN_NAME})

