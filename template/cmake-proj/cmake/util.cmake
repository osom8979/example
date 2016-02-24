# CMake common utility script.

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

# Compiler & IDE:
# CLANG GNU
# MINGW MSYS CYGWIN
# BORLAND WATCOM
# MSVC MSVC_IDE MSVC60 MSVC70 MSVC71 MSVC80 CMAKE_COMPILER_2005 MSVC90 MSVC10

function (print_compiler)
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

#! Append list to list.
#
# \param _value      [out] output value name.
# \param _list_value [in]  list value name.
macro (append_list _value _list_value)
    #message ("_value: ${_value} (${${_value}})")
    #message ("_list_value: ${_list_value} (${${_list_value}})")

    foreach (cursor ${${_list_value}})
        list (APPEND ${_value} ${cursor})
    endforeach ()
endmacro () 

