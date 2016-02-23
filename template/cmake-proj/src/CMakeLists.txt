## CMake main build script.

## ------------------------------------------------------------
set (MAIN_NAME "main" CACHE STRING "Main library name string.")
## ------------------------------------------------------------

project (${MAIN_NAME})

set (${MAIN_NAME}_VERSION_MAJOR  0)
set (${MAIN_NAME}_VERSION_MINOR  0)
set (${MAIN_NAME}_VERSION_PATCH  0)
set (${MAIN_NAME}_VERSION_TWEAK  0)

set (MAIN_SRC_DIR     ".")
set (MAIN_SRC_SUFFIX  "*.cpp")

file (GLOB_RECURSE MAIN_SRCS ${MAIN_SRC_DIR} ${MAIN_SRC_SUFFIX})

add_library (${MAIN_NAME} ${MAIN_SRCS})
#add_dependencies (${MAIN_NAME} ${THIRD_NAME})
#target_link_libraries (${MAIN_NAME} ${THIRD_NAME})

