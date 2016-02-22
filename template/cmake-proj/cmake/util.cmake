# CMake utility script.

## Echo your os.
function (echo_os)
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

