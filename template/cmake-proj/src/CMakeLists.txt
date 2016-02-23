## CMake main build script.

if (MAIN_NAME STREQUAL "")
    message (FATAL_ERROR "Not defined MAIN_NAME variable.")
endif()

project (${MAIN_NAME})

set (${MAIN_NAME}_VERSION_MAJOR  ${VERSION_MAJOR})
set (${MAIN_NAME}_VERSION_MINOR  ${VERSION_MINOR})
set (${MAIN_NAME}_VERSION_PATCH  ${VERSION_PATCH})
set (${MAIN_NAME}_VERSION_TWEAK  ${VERSION_TWEAK})

set (MAIN_SRC_DIR     ".")
set (MAIN_SRC_SUFFIX  "*.cpp")

file (GLOB_RECURSE MAIN_SRCS ${MAIN_SRC_DIR} ${MAIN_SRC_SUFFIX})

add_library (${MAIN_NAME} ${MAIN_SRCS})
#add_dependencies (${MAIN_NAME} ${THIRD_NAME})
#target_link_libraries (${MAIN_NAME} ${THIRD_NAME})

