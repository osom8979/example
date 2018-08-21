#!/bin/sh
# Use the msys-1.0
# Don't use the msysGit.

FILE_NAME=ffmpeg-2.4.1
FILE_EXT=.tar.gz
CONFIG_EXT=--enable-shared

./general_build.sh $FILE_NAME $FILE_EXT "$CONFIG_EXT"
