## CMake build utility script.

include (${PROJECT_SOURCE_DIR}/cmake/util.cmake)

set (BUILD_PROJECT_FILE_PREFIX "project")
set (BUILD_PROJECT_FILE_SUFFIX "cmake")

set (BUILD_PROJECT_FILE_NAME  "${BUILD_PROJECT_FILE_PREFIX}.${BUILD_PROJECT_FILE_SUFFIX}")
set (BUILD_PROJECT_FILE_REGEX "[^/]+/${BUILD_PROJECT_FILE_PREFIX}\\.${BUILD_PROJECT_FILE_SUFFIX}")

set (BUILD_LIB_PROJ_PREFIX  "lib")
set (BUILD_TEST_PROJ_PREFIX "test")
set (BUILD_EXE_PROJ_PREFIX  "exe")

set (BUILD_LIB_PROJ_GLOB  "^${BUILD_LIB_PROJ_PREFIX}${BUILD_PROJECT_FILE_REGEX}$")
set (BUILD_TEST_PROJ_GLOB "^${BUILD_TEST_PROJ_PREFIX}${BUILD_PROJECT_FILE_REGEX}$")
set (BUILD_EXE_PROJ_GLOB  "^${BUILD_EXE_PROJ_PREFIX}${BUILD_PROJECT_FILE_REGEX}$")

#! Find project list.
#
# \param _lib_projs  [out] value name of library project list.
# \param _test_projs [out] value name of test project list.
# \param _exe_projs  [out] value name of executable project list.
function (find_project _lib_projs _test_projs _exe_projs)

    # Find all project.
    file (GLOB_RECURSE _find_project_list RELATIVE "${PROJECT_SOURCE_DIR}" "${BUILD_PROJECT_FILE_NAME}")

    # Find library project.
    list_regex (_find_lib_proj "${BUILD_LIB_PROJ_GLOB}" "${_find_project_list}")
    list (LENGTH _find_lib_proj _find_lib_proj_length)

    # Remove library project in the all project.
    if (${_find_lib_proj_length} GREATER 0)
        list (REMOVE_ITEM _find_project_list ${_find_lib_proj})
    endif ()

    # Find test executable project.
    list_regex (_find_test_proj "${BUILD_TEST_PROJ_GLOB}" "${_find_project_list}")
    list (LENGTH _find_test_proj _find_test_proj_length)

    # Remove test project in the all project.
    if (${_find_test_proj_length} GREATER 0)
        list (REMOVE_ITEM _find_project_list ${_find_test_proj})
    endif ()

    # Find executable project.
    list_regex (_find_exe_proj "${BUILD_EXE_PROJ_GLOB}" "${_find_project_list}")

    string (REPLACE "/${BUILD_PROJECT_FILE_NAME}" "" ${_lib_projs}  "${_find_lib_proj}")
    string (REPLACE "/${BUILD_PROJECT_FILE_NAME}" "" ${_test_projs} "${_find_test_proj}")
    string (REPLACE "/${BUILD_PROJECT_FILE_NAME}" "" ${_exe_projs}  "${_find_exe_proj}")

    set (${_lib_projs}  ${${_lib_projs}}  PARENT_SCOPE)
    set (${_test_projs} ${${_test_projs}} PARENT_SCOPE)
    set (${_exe_projs}  ${${_exe_projs}}  PARENT_SCOPE)
endfunction ()

set (BUILD_PROJECT_TYPE_LIB      "${BUILD_LIB_PROJ_PREFIX}")
set (BUILD_PROJECT_TYPE_TEST     "${BUILD_TEST_PROJ_PREFIX}")
set (BUILD_PROJECT_TYPE_EXE      "${BUILD_EXE_PROJ_PREFIX}")
set (BUILD_PROJECT_TYPE_UNKNOWN  "unknown")

#! Check the project type.
#
# \param _value [out] value name of result type string.
function (get_project_type _value _project_dir_name)
    if (${_project_dir_name} MATCHES "^${BUILD_LIB_PROJ_PREFIX}.+")
        set (${_value} "${BUILD_PROJECT_TYPE_LIB}" PARENT_SCOPE)
    elseif (${_project_dir_name} MATCHES "^${BUILD_TEST_PROJ_PREFIX}.+")
        set (${_value} "${BUILD_PROJECT_TYPE_TEST}" PARENT_SCOPE)
    elseif (${_project_dir_name} MATCHES "^${BUILD_EXE_PROJ_PREFIX}.+")
        set (${_value} "${BUILD_PROJECT_TYPE_EXE}" PARENT_SCOPE)
    else ()
        set (${_value} "${BUILD_PROJECT_TYPE_UNKNOWN}" PARENT_SCOPE)
    endif ()
endfunction()

#! Default build process.
#
# \param _libs  [in] List of library.
# \param _tests [in] List of test.
# \param _exes  [in] List of executable.
function (default_build _libs _tests _exes)
    # Build project.
    foreach (_cursor ${_libs} ${_tests} ${_exes})
        set (_project_is_verbose ON)
        set (_project_objects)
        set (_project_libraries)
        set (_project_dependencies)
        set (_project_cflags)
        set (_project_cxxflags)
        set (_project_ldflags)

        set (_project_dir ${_cursor})
        get_project_type (_project_type "${_cursor}")
        string (REPLACE "${_project_type}" "" _project_name ${_cursor})

        if (_project_is_verbose)
            message ("++ Project type: ${_project_type}")
        endif ()

        project (${_project_name})

        # Compile files.
        find_compile_object (_project_objects _project_libraries "${PROJECT_SOURCE_DIR}/${_project_dir}")

        # User defined sub-project settings.
        include ("${PROJECT_SOURCE_DIR}/${_project_dir}/${BUILD_PROJECT_FILE_NAME}")

        # After settings.
        set  (_project_total_libs ${_project_dependencies} ${_project_libraries})
        list (LENGTH _project_total_libs _project_total_libs_length)
        list (LENGTH _project_dependencies _project_dependencies_length)
        list (LENGTH _project_objects _project_objects_length)

        # Dependencies setting.
        if (${_project_dependencies_length} GREATER 0)
            if (_project_is_verbose)
                message ("++ Dependencies: ${_project_dependencies}")
            endif ()
            add_dependencies (${_project_name} ${_project_dependencies})
        endif ()

        # Libraries setting.
        if (${_project_total_libs_length} GREATER 0)
            if (_project_is_verbose)
                message ("++ Libraries: ${_project_total_libs}")
            endif ()
            target_link_libraries (${_project_name} ${_project_total_libs})
        endif ()

        if (${_project_objects_length} GREATER 0)
            if ("${_project_type}" STREQUAL "${BUILD_PROJECT_TYPE_LIB}")
                add_library (${_project_name} ${_project_objects})
            else ()
                add_executable (${_project_name} ${_project_objects})
            endif ()
        else ()
            message (FATAL_ERROR "Not found ${_project_name} object files.")
        endif ()
    endforeach ()
endfunction ()

