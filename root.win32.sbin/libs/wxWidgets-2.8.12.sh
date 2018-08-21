#!/bin/sh

FILE_NAME=wxWidgets-3.0.1
FILE_EXT=.tar.bz2

CONFIG_EXT="$CONFIG_EXT --disable-shared"
CONFIG_EXT="$CONFIG_EXT --disable-debug"
CONFIG_EXT="$CONFIG_EXT --disable-threads"
CONFIG_EXT="$CONFIG_EXT --enable-monolithic"
CONFIG_EXT="$CONFIG_EXT --enable-unicode"
CONFIG_EXT="$CONFIG_EXT --without-libjpeg"
CONFIG_EXT="$CONFIG_EXT --without-libpng"
CONFIG_EXT="$CONFIG_EXT --without-zlib"
CONFIG_EXT="$CONFIG_EXT --without-libtiff"
CONFIG_EXT="$CONFIG_EXT --without-expat"
CONFIG_EXT="$CONFIG_EXT --without-regex"

export MAKE=mingw32-make
export CXXFLAGS="-fpermissive"

./general_build.sh $FILE_NAME $FILE_EXT "$CONFIG_EXT"
