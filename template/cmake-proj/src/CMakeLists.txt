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
         LIBRARY DESTINATION lib)
install (DIRECTORY ${MAIN_NAME}/
         DESTINATION include/${MAIN_NAME}
         FILES_MATCHING PATTERN "*.h")

set (CPACK_GENERATOR "DEB")
set (CPACK_PACKAGE_VERSION_MAJOR        "${${MAIN_NAME}_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR        "${${MAIN_NAME}_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH        "${${MAIN_NAME}_VERSION_PATCH}")
set (CPACK_PACKAGE_NAME                 "${MAIN_NAME}")
set (CPACK_PACKAGE_DESCRIPTION          "${MAIN_BRIEF}")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY  "${MAIN_BRIEF}")
set (CPACK_PACKAGE_VENDOR               "${MAIN_AUTHOR}")
set (CPACK_PACKAGE_CONTACT              "${MAIN_EMAIL}")
set (CPACK_PACKAGING_INSTALL_PREFIX     "/usr/local")

## Debian settings.
set (CPACK_DEBIAN_PACKAGE_DEPENDS "zlib1g-dev")
#set (CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/debian/postinst")

include (CPack)

