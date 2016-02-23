## CMake test build script.

if (MAIN_NAME STREQUAL "")
    message (FATAL_ERROR "Not found MAIN_NAME variable.")
endif()

## --------------------------------------------------------------
set (TEST_NAME "tester" CACHE STRING "Test library name string.")
## --------------------------------------------------------------

project (${TEST_NAME})

set (TEST_SRC_DIR     ".")
set (TEST_SRC_SUFFIX  "*.cpp")

file (GLOB_RECURSE TEST_SRCS ${TEST_SRC_DIR} ${TEST_SRC_SUFFIX})

add_executable (${TEST_NAME} ${TEST_SRCS})
add_dependencies (${TEST_NAME} ${MAIN_NAME})
target_link_libraries (${TEST_NAME} ${MAIN_NAME})

