## CMake main build script.

exists_define_or_die (MAIN_NAME)

project (${MAIN_NAME})

set (${MAIN_NAME}_VERSION_MAJOR  ${VERSION_MAJOR})
set (${MAIN_NAME}_VERSION_MINOR  ${VERSION_MINOR})
set (${MAIN_NAME}_VERSION_PATCH  ${VERSION_PATCH})
set (${MAIN_NAME}_VERSION_TWEAK  ${VERSION_TWEAK})
set (${MAIN_NAME}_VERSION        ${VERSION})

find_compile_object (_objects _libraries "${CMAKE_CURRENT_SOURCE_DIR}")

add_library (${MAIN_NAME} ${_objects})
#add_dependencies (${MAIN_NAME} ${THIRD_NAME})
target_link_libraries (${MAIN_NAME} ${_libraries})

# Install ${CMAKE_INSTALL_PREFIX} settings.
install (TARGETS ${MAIN_NAME}
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
install (DIRECTORY ${MAIN_NAME}/
         DESTINATION include/${MAIN_NAME}
         FILES_MATCHING PATTERN "*.h")

