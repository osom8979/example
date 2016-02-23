## CMake test build script.

exists_define (TEST_NAME)

project (${TEST_NAME})

set (TEST_SRC_DIR     ".")
set (TEST_SRC_SUFFIX  "*.cpp")

file (GLOB_RECURSE TEST_SRCS ${TEST_SRC_DIR} ${TEST_SRC_SUFFIX})

add_executable (${TEST_NAME} ${TEST_SRCS})
add_dependencies (${TEST_NAME} ${MAIN_NAME})
target_link_libraries (${TEST_NAME} ${MAIN_NAME})

