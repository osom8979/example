#!/bin/sh

FILE_NAME=SDL-9f8a2b28f61d
FILE_EXT=.tar.gz
CONFIG_EXT="--enable-shared --disable-directx"

./general_build.sh $FILE_NAME $FILE_EXT "$CONFIG_EXT"
