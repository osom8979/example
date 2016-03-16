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

# --------------------
# Solution properties.
# --------------------

# Information.
set (MAIN_NAME   "main"    CACHE STRING  "Solution name string.")
set (MAIN_AUTHOR "author"  CACHE STRING  "Solution author.")
set (MAIN_EMAIL  "email"   CACHE STRING  "Author E-mail address.")
set (MAIN_BRIEF  "brief"   CACHE STRING  "Solution brief string.")

# Libraries.
set (LIST_OF_LIBRARIES)
# Default project:
add_library_value ("zlib"      ON   LIST_OF_LIBRARIES)
add_library_value ("gtest"     ON   LIST_OF_LIBRARIES)
add_library_value ("gflags"    ON   LIST_OF_LIBRARIES)
add_library_value ("glog"      ON   LIST_OF_LIBRARIES)
add_library_value ("protobuf"  ON   LIST_OF_LIBRARIES)
# Extension project:
add_library_value ("lmdb"      OFF  LIST_OF_LIBRARIES)
add_library_value ("hdf5"      OFF  LIST_OF_LIBRARIES)
add_library_value ("openblas"  OFF  LIST_OF_LIBRARIES)
add_library_value ("libuv"     OFF  LIST_OF_LIBRARIES)
add_library_value ("snappy"    OFF  LIST_OF_LIBRARIES)
add_library_value ("leveldb"   OFF  LIST_OF_LIBRARIES)
add_library_value ("libpng"    OFF  LIST_OF_LIBRARIES)
# Massive project:
add_library_value ("boost"     OFF  LIST_OF_LIBRARIES)
add_library_value ("ffmpeg"    OFF  LIST_OF_LIBRARIES)
add_library_value ("wxwidgets" OFF  LIST_OF_LIBRARIES)
add_library_value ("sdl2"      OFF  LIST_OF_LIBRARIES)
add_library_value ("opencv3"   OFF  LIST_OF_LIBRARIES)
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

