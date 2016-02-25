# CMake linker utility script.

#! Insert whole-archive flags.
#
# \param _value [out] value name of output list.
macro (insert_whole_archive_flags _value)
    #message ("_value: ${_value} (${${_value}})")

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        list (INSERT ${_value} 0 "-Wl,-force_load")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        list (INSERT ${_value} 0 "-Wl,--whole-archive")
        list (APPEND ${_value} "-Wl,--no-whole-archive")
    endif ()
endmacro ()

