## CMake test  build script.

## -----------------------------
## Enter the name of the sample.
set (SAMPLE_NAME "sample")
## -----------------------------

project (${SAMPLE_NAME})

set (${SAMPLE_NAME}_VERSION_MAJOR  ${MAIN_NAME}_VERSION_MAJOR)
set (${SAMPLE_NAME}_VERSION_MINOR  ${MAIN_NAME}_VERSION_MINOR)
set (${SAMPLE_NAME}_VERSION_PATCH  ${MAIN_NAME}_VERSION_PATCH)
set (${SAMPLE_NAME}_VERSION_TWEAK  ${MAIN_NAME}_VERSION_TWEAK)

add_executable (${SAMPLE_NAME} main.cpp)
add_dependencies (${SAMPLE_NAME} ${MAIN_NAME})
target_link_libraries (${SAMPLE_NAME} ${MAIN_NAME})

