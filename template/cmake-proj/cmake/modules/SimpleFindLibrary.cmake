## Find library macro.

#! Find {Library} macro.
#
# The following variables are optionally searched for defaults.
#  ${_prefix}_ROOT
#  ${_prefix}_ROOT_INCLUDE_PATHS
#  ${_prefix}_ROOT_LIBRARY_PATHS
#
# The following are set after configuration is done:
#  ${_prefix}_FOUND
#  ${_prefix}_INCLUDE_DIRS
#  ${_prefix}_LIBRARIES
#
# \param _prefix  [in] Variable result prefix.
# \param _headers [in] Find header files.
# \param _libs    [in] Find library files.
macro (simple_find_library _prefix  _headers _libs)

    set (_root_dir           "${_prefix}_ROOT")
    set (_root_include_paths "${_prefix}_ROOT_INCLUDE_PATHS")
    set (_root_library_paths "${_prefix}_ROOT_LIBRARY_PATHS")

    # Check 64bit library directories.
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set (_lib64_paths "/usr/lib64"
                          "/usr/local/lib64"
                          "${${_root_dir}}/lib64"
                          "$ENV{${_root_dir}}/lib64")
    else ()
        set (_lib64_paths)
    endif ()

    set (_search_include_paths "/usr/include"
                               "/usr/local/include"
                               "${${_root_dir}}"
                               "${${_root_dir}}/include"
                               "$ENV{${_root_dir}}"
                               "$ENV{${_root_dir}}/include"
                               ${_root_include_paths})
    set (_search_library_paths "/usr/lib"
                               "/usr/local/lib"
                               "${${_root_dir}}"
                               "${${_root_dir}}/lib"
                               "$ENV{${_root_dir}}"
                               "$ENV{${_root_dir}}/lib"
                               ${_root_library_paths}
                               ${_lib64_paths})

    find_path (_looked_for_include_dir
        NAMES ${_headers}
        PATHS ${_search_include_paths})

    find_library (_looked_for_library
        NAMES ${_libs}
        PATHS ${_search_library_paths}
        PATH_SUFFIXES Debug Release)

    set (LOOKED_FOR _looked_for_library _looked_for_include_dir)

    include (FindPackageHandleStandardArgs)
    find_package_handle_standard_args (${_prefix} DEFAULT_MSG ${LOOKED_FOR})

    if (${_prefix}_FOUND)
        set (${_prefix}_INCLUDE_DIRS ${_looked_for_include_dir})
        set (${_prefix}_LIBRARIES    ${_looked_for_library})
        mark_as_advanced (${LOOKED_FOR})
        # message (STATUS "Found ${_prefix} (include: ${_looked_for_include_dir}, library: ${_looked_for_library})")
    endif ()

endmacro ()
