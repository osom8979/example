## CMake library utility script.

#! Register project.
#
# \param _name    [in] project name.
# \param _is_lib  [in] is library project?
# \param _is_test [in] is test project?
# \param _is_exe  [in] is executable project?
macro (register_project _name _is_lib _is_test _is_exe)
    set (${_name}_project_name     "${_name}")
    set (${_name}_project_is_lib   ${_is_lib})
    set (${_name}_project_is_test  ${_is_test})
    set (${_name}_project_is_exe   ${_is_exe})
    project (${_name})
endmacro ()

#! Default build macro.
#
# \param _libs  [in] list of library.
# \param _tests [in] list of test.
# \param _exes  [in] list of executable.
macro (default_build _libs _tests _exes)
    set (_proj_list ${_libs} ${_tests} ${_exes})

    # Register project.
    foreach (_cursor ${_libs})
        register_project (${_cursor} ON OFF OFF)
    endforeach ()
    foreach (_cursor ${_tests})
        register_project (${_cursor} OFF ON OFF)
    endforeach ()
    foreach (_cursor ${_exes})
        register_project (${_cursor} OFF OFF ON)
    endforeach ()

    # Build project.
    foreach (_cursor ${_proj_list})
        set (_project_name    ${_cursor})
        set (_project_is_lib  ${${_cursor}_project_is_lib})
        set (_project_is_test ${${_cursor}_project_is_test})
        set (_project_is_exe  ${${_cursor}_project_is_exe})
        set (_project_objects)
        set (_project_libraries)
        set (_project_dependencies)
        set (_project_cflags)
        set (_project_cxxflags)
        set (_project_ldflags)
        set (_project_is_verbose OFF)

        find_compile_object (_project_objects _project_libraries "${PROJECT_SOURCE_DIR}/${_cursor}")

        # User defined sub-project settings.
        include ("${PROJECT_SOURCE_DIR}/${_project_name}/${_PROJECT_NAME}")

        # After settings.
        set  (_project_total_libs ${_project_dependencies} ${_project_libraries})
        list (LENGTH _project_total_libs _project_total_libs_length)
        list (LENGTH _project_dependencies _project_dependencies_length)

        if (${_project_dependencies_length} GREATER 0)
            add_dependencies (${_project_name} ${_project_dependencies})
        endif ()

        if (${_project_total_libs_length} GREATER 0)
            target_link_libraries (${_project_name} ${_project_total_libs})
        endif ()

        if (_project_is_lib)
            add_library (${_project_name} ${_project_objects})
        else ()
            add_executable (${_project_name} ${_project_objects})
        endif ()
    endforeach ()
endmacro ()

