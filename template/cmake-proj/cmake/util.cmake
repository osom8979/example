## CMake common utility script.

if (WIN32 AND NOT MSYS AND NOT CYGWIN)
    set (PATH_SEPARATOR ";")
else ()
    set (PATH_SEPARATOR ":")
endif ()

## -------------------
## System information.
## -------------------

#! Print your os.
function (print_os)
    if (UNIX)
        # including Apple OS X and CygWin.
        message ("Unix-like OS's.")
    endif ()
    if (WIN32)
        # Prior to 2.8.4 this included CygWin.
        message ("Windows OS.")
    endif ()
    if (APPLE)
        message ("Apple systems.")
    endif ()
endfunction ()

function (print_compiler)
    # Compiler & IDE:
    # CLANG GNU
    # MINGW MSYS CYGWIN
    # BORLAND WATCOM
    # MSVC MSVC_IDE MSVC60 MSVC70 MSVC71 MSVC80 CMAKE_COMPILER_2005 MSVC90 MSVC10
    message (WARNING "Not implement.")
endfunction ()

#! Print your compiler.
function (print_compiler_id)
    message ("Compiler id: ${CMAKE_CXX_COMPILER_ID}")
endfunction ()

#! Check the exists define.
# If not exists, it send a fatal error.
#
# \param _value [in] check value name.
function (exists_define_or_die _value)
    if (NOT DEFINED ${_value} OR ${_value} STREQUAL "")
        message (FATAL_ERROR "Not defined ${_value} variable.")
    endif()
endfunction ()

## -----------------
## Module utilities.
## -----------------

#! Test find_package() function.
#
# \param _enable      [in] Enable variable.
# \param _module_name [in] Module name.
macro (test_find_package _enable _module_name)
    if (USE_${_enable})
        find_package (${_module_name})
    endif ()
endmacro ()

## ------------------
## String operations.
## ------------------

#! List of regex.
#
# \param _value [out] output value name.
# \param _regex [in]  Regex string.
# \param _list  [in]  Original list.
function (list_regex _value _regex _list)
    set (${_value})
    foreach (_cursor ${_list})
        string (REGEX MATCH "${_regex}" _match_cursor ${_cursor})
        list (APPEND ${_value} ${_match_cursor})
    endforeach ()
    set (${_value} ${${_value}} PARENT_SCOPE)
endfunction ()

