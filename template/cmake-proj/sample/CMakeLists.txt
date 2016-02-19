
set (SAMPLE_NAME "sample")

project (${SAMPLE_NAME})

add_executable (${SAMPLE_NAME} main.cpp)

add_dependencies (${SAMPLE_NAME} ${MAIN_NAME})

target_link_libraries (${SAMPLE_NAME} ${MAIN_NAME})

