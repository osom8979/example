#!/bin/sh

FILE_NAME=ncurses-5.9
FILE_EXT=.tar.gz

CONFIG_EXT="--without-ada"
CONFIG_EXT="$CONFIG_EXT --disable-home-terminfo"
CONFIG_EXT="$CONFIG_EXT --enable-reentrant"
CONFIG_EXT="$CONFIG_EXT --enable-sp-funcs"
CONFIG_EXT="$CONFIG_EXT --enable-term-driver"
CONFIG_EXT="$CONFIG_EXT --enable-interop"
#CONFIG_EXT="$CONFIG_EXT --with-pthread"

./general_build.sh $FILE_NAME $FILE_EXT "$CONFIG_EXT"
