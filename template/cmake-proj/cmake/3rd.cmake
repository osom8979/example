## CMake third-party library script.

#! Build third-party libraries.
#
# \param _lib_list [in] list of library name.
# \param _prefix   [in] install prefix.
function (build_3rd _lib_list _prefix)

    set (_temp_prefix     "${_prefix}/temp")
    set (_include_prefix  "${_prefix}/include")
    set (_library_prefix  "${_prefix}/lib")

    if (NOT EXISTS ${_temp_prefix})
        file (MAKE_DIRECTORY ${_temp_prefix})
    endif ()

    # loop of library.
    foreach (_cursor ${_lib_list})
        set (_script_path "${PROJECT_SOURCE_DIR}/cmake/libraries/${_cursor}.cmake")

        if (USE_${_cursor})
            if (EXISTS "${_script_path}")
                message ("** Use the ${_cursor} library.")
                execute_process (
                    COMMAND ${CMAKE_COMMAND}
                    -DVERBOSE=OFF
                    "-DWORKING_DIR=${_temp_prefix}"
                    "-DINSTALL_PREFIX=${_prefix}"
                    "-DINCLUDE_PATH=${_include_prefix}"
                    "-DLIBRARY_PATH=${_library_prefix}"
                    "-DCMAKE_STATIC_LIBRARY_PREFIX=${CMAKE_STATIC_LIBRARY_PREFIX}"
                    "-DCMAKE_STATIC_LIBRARY_SUFFIX=${CMAKE_STATIC_LIBRARY_SUFFIX}"
                    "-DCMAKE_EXECUTABLE_SUFFIX=${CMAKE_EXECUTABLE_SUFFIX}"
                    -P "${_script_path}")
            else ()
                message ("** Not found ${_script_path}.")
            endif ()
        else ()
            message ("** Disable ${_cursor} library.")
        endif ()
    endforeach ()
endfunction ()

