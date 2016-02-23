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
# \param value [in] check value name.
function (exists_define_or_die value)
    if (NOT DEFINED ${value} OR ${value} STREQUAL "")
        message (FATAL_ERROR "Not defined ${value} variable.")
    endif()
endfunction ()

#! Append list to list.
#
# \param value      [out] output value name.
# \param list_value [in]  list value name.
macro (append_list value list_value)
    #message ("value: ${value} (${${value}}), list_value: ${list_value} (${${list_value}})")

    foreach (cursor ${${list_value}})
        list (APPEND ${value} ${cursor})
    endforeach ()
endmacro () 

