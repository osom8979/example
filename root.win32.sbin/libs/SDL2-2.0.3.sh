#!/bin/sh

FILE_NAME=SDL2-2.0.3
FILE_EXT=.tar.gz
CONFIG_EXT="--enable-shared --disable-directx"

## SDL_platform.h: fatal error: winapifamily.h: No such file or directory
## #include <winapifamily.h>
##
## C:\Program Files (x86)\Windows Kits\8.1\Include\shared
## OR
## C:\Program Files (x86)\Windows Kits\8.0\Include\shared

# Convert to the unix style paths:
if [ -d $WINDOWS_KIT_HOME ]; then
export CPPFLAGS=`echo $WINDOWS_KIT_INCS \
    | sed 's;\\([a-z,A-Z]\\):\\\\;-I/\\1/;g' \
    | sed 's;\\\\\\\\;\\\\;g' \
    | sed 's;\\\\;/;g' \
    | sed 's/[;:]/ /g'`
export LDFLAGS=`echo $WINDOWS_KIT_LIBS \
    | sed 's;\\([a-z,A-Z]\\):\\\\;-L/\\1/;g' \
    | sed 's;\\\\\\\\;\\\\;g' \
    | sed 's;\\\\;/;g' \
    | sed 's/[;:]/ /g'`
fi

#echo "CPPFLAGS=$CPPFLAGS"
#echo "LDFLAGS=$LDFLAGS"

export MAKE=make

./general_build.sh $FILE_NAME $FILE_EXT "$CONFIG_EXT"
