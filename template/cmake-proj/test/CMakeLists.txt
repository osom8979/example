## CMake test  build script.

set (TEST_NAME "tester")

project (${TEST_NAME})

set (${TEST_NAME}_VERSION_MAJOR  ${MAIN_NAME}_VERSION_MAJOR)
set (${TEST_NAME}_VERSION_MINOR  ${MAIN_NAME}_VERSION_MINOR)
set (${TEST_NAME}_VERSION_PATCH  ${MAIN_NAME}_VERSION_PATCH)
set (${TEST_NAME}_VERSION_TWEAK  ${MAIN_NAME}_VERSION_TWEAK)

add_executable (${TEST_NAME} main.cpp)
add_dependencies (${TEST_NAME} ${MAIN_NAME})
target_link_libraries (${TEST_NAME} ${MAIN_NAME})
