#!/bin/sh

FILE_NAME=wxWidgets-3.0.1
FILE_EXT=.tar.bz2
CONFIG_EXT="--disable-static --enable-shared"

./general_build.sh $FILE_NAME $FILE_EXT "$CONFIG_EXT"
