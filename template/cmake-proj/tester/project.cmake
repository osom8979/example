## CMake subproject properties.

if (_project_is_verbose)
    message ("++ _project_dir: ${_project_dir}")
    message ("++ _project_type: ${_project_type}")
    message ("++ _project_name: ${_project_name}")
    message ("++ _project_objects: ${_project_objects}")
    message ("++ _project_libraries: ${_project_libraries}")
    message ("++ _project_dependencies: ${_project_dependencies}")
    message ("++ _project_cflags: ${_project_cflags}")
    message ("++ _project_cxxflags: ${_project_cxxflags}")
    message ("++ _project_ldflags: ${_project_ldflags}")
endif ()
