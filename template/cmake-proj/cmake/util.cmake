# CMake utility script.

## Print your os.
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
# MINGW MSYS CYGWIN BORLAND WATCOM MSVC MSVC_IDE MSVC60 MSVC70 MSVC71 MSVC80 CMAKE_COMPILER_2005 MSVC90 MSVC10

## Print your compiler.
function (print_compiler_id)
    message ("Compiler id: ${CMAKE_CXX_COMPILER_ID}")
endfunction ()

