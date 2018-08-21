#!/bin/sh

FILE_NAME=x264-snapshot-20140929-2245
FILE_EXT=.tar.gz
CONFIG_EXT="--disable-static --enable-shared --enable-win32thread"

export MAKE=mingw32-make

./general_build.sh $FILE_NAME $FILE_EXT "$CONFIG_EXT"
