## CMake sample build script.

exists_define_or_die (SAMPLE_NAME)

project (${SAMPLE_NAME})

set (SAMPLE_SRC_DIR     ".")
set (SAMPLE_SRC_SUFFIX  "*.cpp")

file (GLOB_RECURSE SAMPLE_SRCS ${SAMPLE_SRC_DIR} ${SAMPLE_SRC_SUFFIX})

add_executable (${SAMPLE_NAME} ${SAMPLE_SRCS})
add_dependencies (${SAMPLE_NAME} ${MAIN_NAME})
target_link_libraries (${SAMPLE_NAME} ${MAIN_NAME})

