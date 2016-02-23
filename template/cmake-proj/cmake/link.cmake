# CMake linker utility script.

#! Insert whole-archive flags.
#
# \param value [out] output list value name.
macro (insert_whole_archive_flags value)
    #message ("value: ${value} (${${value}})")

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        list (INSERT ${value} 0 "-Wl,-force_load")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        list (INSERT ${value} 0 "-Wl,--whole-archive")
        list (APPEND ${value} "-Wl,--no-whole-archive")
    endif ()
endmacro ()

