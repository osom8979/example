## CMake main build script.

exists_define_or_die (MAIN_NAME)

project (${MAIN_NAME})

set (${MAIN_NAME}_VERSION_MAJOR  ${VERSION_MAJOR})
set (${MAIN_NAME}_VERSION_MINOR  ${VERSION_MINOR})
set (${MAIN_NAME}_VERSION_PATCH  ${VERSION_PATCH})
set (${MAIN_NAME}_VERSION_TWEAK  ${VERSION_TWEAK})

set (MAIN_SRC_DIR       ".")
set (MAIN_SRC_SUFFIX    "*.cpp")
set (MAIN_PROTO_SUFFIX  "*.proto")

file (GLOB_RECURSE MAIN_SRCS    ${MAIN_SRC_SUFFIX})
file (GLOB_RECURSE MAIN_PROTOS  ${MAIN_PROTO_SUFFIX})

set (MAIN_LIBS)
set (MAIN_STATIC_LIBS "${THIRD_PREFIX}/lib/libprotobuf.a"
                      "${THIRD_PREFIX}/lib/libz.a")

insert_whole_archive_flags (MAIN_STATIC_LIBS)

list (APPEND MAIN_LIBS ${MAIN_STATIC_LIBS})

#add_dependencies (${MAIN_NAME} ${THIRD_NAME})
#target_link_libraries (${MAIN_NAME} ${THIRD_NAME})

protobuf_generate_cpp_bugfix (MAIN_PROTO_SRCS MAIN_PROTO_HEADERS "${MAIN_PROTOS}")
#message ("srcs: ${MAIN_PROTO_SRCS}")
#message ("hdrs: ${MAIN_PROTO_HEADERS}")

add_library (${MAIN_NAME} ${MAIN_SRCS} ${MAIN_PROTO_SRCS})
target_link_libraries (${MAIN_NAME} ${MAIN_LIBS})

