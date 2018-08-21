#!/bin/sh

# File does not exist.
FILE_NAME=cmake-3.0.2
FILE_EXT=.zip
CONFIG_EXT="--prefix=$PREFIX"

./general_build.sh $FILE_NAME $FILE_EXT "$CONFIG_EXT"
