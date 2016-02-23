## CMake sample build script.

if (SAMPLE_NAME STREQUAL "")
    message (FATAL_ERROR "Not defined SAMPLE_NAME variable.")
endif()

project (${SAMPLE_NAME})

set (SAMPLE_SRC_DIR     ".")
set (SAMPLE_SRC_SUFFIX  "*.cpp")

file (GLOB_RECURSE SAMPLE_SRCS ${SAMPLE_SRC_DIR} ${SAMPLE_SRC_SUFFIX})

add_executable (${SAMPLE_NAME} ${SAMPLE_SRCS})
add_dependencies (${SAMPLE_NAME} ${MAIN_NAME})
target_link_libraries (${SAMPLE_NAME} ${MAIN_NAME})

