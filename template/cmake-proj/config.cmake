## CMake configure script.

#! Add a library value.
#
# \param _name        [in] Library name.
# \param _enable      [in] Enable(ON) or disable(OFF).
# \param _output_list [in] Value name of output list.
macro (add_library_value _name _enable _output_list)
    set (USE_${_name} ${_enable} CACHE BOOL "Use the ${_name} library.")
    if (USE_${_name})
        list (APPEND ${_output_list} "${_name}")
    endif ()
endmacro ()

# Information.
set (MAIN_NAME   "main"       CACHE STRING  "Solution name string.")
set (MAIN_BRIEF  "brief"      CACHE STRING  "Solution brief string.")
set (MAIN_AUTHOR "anonymous"  CACHE STRING  "Solution author.")
set (MAIN_EMAIL  "email"      CACHE STRING  "Author E-mail address.")

# Libraries.
set (LIST_OF_LIBRARIES)
add_library_value ("gtest"     ON  LIST_OF_LIBRARIES)
add_library_value ("zlib"      ON  LIST_OF_LIBRARIES)
add_library_value ("protobuf"  ON  LIST_OF_LIBRARIES)
add_library_value ("gflags"    ON  LIST_OF_LIBRARIES)
set (LIST_OF_LIBRARIES ${LIST_OF_LIBRARIES})

# Main version.
set (VERSION_MAJOR  0)
set (VERSION_MINOR  0)
set (VERSION_PATCH  0)

# Packet version.
set (VERSION_PACKET_MAJOR  0)
set (VERSION_PACKET_MINOR  0)

# Extension version number.
set (VERSION_RELEASE  0)

