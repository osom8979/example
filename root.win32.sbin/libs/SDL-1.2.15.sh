#!/bin/sh

FILE_NAME=SDL-1.2.15
FILE_EXT=.tar.gz
#CONFIG_EXT="--enable-shared --disable-directx --disable-haptic --disable-static"
CONFIG_EXT=

./general_build.sh $FILE_NAME $FILE_EXT "$CONFIG_EXT"
