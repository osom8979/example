#!/bin/sh

FILE_NAME=irrlicht-1.8.1
FILE_EXT=.zip

WORKING_DIR=$PWD
FILE_PARENT=/var/opm/cache/libs

if [ $AM64 -eq 0 ]; then
TEMP_DIR=$TMP/libs
else
TEMP_DIR=$TMP/libs64
fi

FILE_PATH=$FILE_PARENT/$FILE_NAME$FILE_EXT
DEST_PATH=$TEMP_DIR

DATE_FORMAT=`date +%Y%m%d_%H-%M-%S`
LOG_FILE=$FILE_NAME-$DATE_FORMAT.log
LOG_PATH=$DEST_PATH/$LOG_FILE

EXTRACT_COMMAND=unzip
EXTRACT_OPTIONS=-o
MAKE=make

## --------------
## Function list.
## --------------

function checkExitError()
{
    local ERROR_CODE=$?

    if [ $ERROR_CODE -ne 0 ]; then
        echo "Error signal ($ERROR_CODE)"
        exit $ERROR_CODE
    fi
}

## -------------
## Main scripts.
## -------------

if [ ! -d $TEMP_DIR ]; then
    mkdir -p "$TEMP_DIR"
fi

export CC=gcc
export CXX=g++
export CXXFLAGS="-w -D__GNUWIN32__=1"

## ------------
## Start build.
## ------------

cd $DEST_PATH

echo "- extract."
$EXTRACT_COMMAND $EXTRACT_OPTIONS $FILE_PATH >> $LOG_PATH
cd $FILE_NAME/source/Irrlicht

checkExitError
echo "- patch."
patch -l -p3 < $WORKING_DIR/$FILE_NAME.patch >> $LOG_PATH

checkExitError
echo "- build."
#$MAKE all sharedlib_win32 >> $LOG_PATH
$MAKE all >> $LOG_PATH

checkExitError
echo "- install."
cp *.a $LLIB
cd ../..
mkdir -p $LINC/irrlicht
cp -r include/* $LINC/irrlicht
mkdir -p $PREFIX/doc/$FILE_NAME
cp -r doc/* $PREFIX/doc/$FILE_NAME
cp bin/Win32-gcc/Irrlicht.dll $LBIN
cp lib/Win32-gcc/libIrrlicht.a $LLIB
cp lib/Win32-gcc/libIrrlicht.dll.a $LLIB
cp lib/Win32-gcc/libIrrlicht.def $LLIB

echo "- end! ($?)"
cd $WORKING_DIR
