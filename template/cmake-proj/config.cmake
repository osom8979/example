## CMake configure script.

#! Add a library value.
#
# \param _name        [in] Library name.
# \param _enable      [in] Enable(ON) or disable(OFF).
macro (add_library_value _name _enable)
    set (USE_${_name} ${_enable} CACHE BOOL "Use the ${_name} library.")
endmacro ()

# --------------------
# Solution properties.
# --------------------

# Information.
set (MAIN_NAME   "libmain" CACHE STRING  "Solution name string.")
set (MAIN_AUTHOR "author"  CACHE STRING  "Solution author.")
set (MAIN_EMAIL  "email"   CACHE STRING  "Author E-mail address.")
set (MAIN_BRIEF  "brief"   CACHE STRING  "Solution brief string.")

# Libraries.
add_library_value ("zlib"      ON)
add_library_value ("gtest"     ON)
add_library_value ("gflags"    ON)
add_library_value ("glog"      ON)
add_library_value ("protobuf"  ON)
# Extension project:
add_library_value ("lmdb"      OFF)
add_library_value ("hdf5"      OFF)
add_library_value ("openblas"  OFF)
add_library_value ("libuv"     OFF)
add_library_value ("snappy"    OFF)
add_library_value ("leveldb"   OFF)
add_library_value ("libpng"    OFF)
# Massive project:
add_library_value ("boost"     OFF)
add_library_value ("ffmpeg"    OFF)
add_library_value ("wxwidgets" OFF)
add_library_value ("sdl2"      OFF)
add_library_value ("opencv3"   OFF)
# Custom project:
add_library_value ("tbag"      OFF)

# Compiler extension.
set (USE_CUDA       ON  CACHE BOOL "Use the nvidia-cuda.")
set (USE_CUBLAS     ON  CACHE BOOL "Use the CUDA BLAS library.")
set (USE_CUDNN      ON  CACHE BOOL "Use the cuDNN.")
set (USE_OBFUSCATE  ON  CACHE BOOL "Use the obfuscate.")

# Continuous Integration.
option (USE_DOXYGEN "API documentation." ON)

# 3rd-party library prefix.
set (THIRD_PREFIX "$ENV{TPARTY_HOME}/local" CACHE STRING "3rd-party library prefix")

# Main version.
set (VERSION_MAJOR  0)
set (VERSION_MINOR  0)
set (VERSION_PATCH  0)

# Packet version.
set (VERSION_PACKET_MAJOR  0)
set (VERSION_PACKET_MINOR  0)

# Extension version number.
set (VERSION_RELEASE  0)

