#!/bin/sh

FILE_NAME=opencv-2.4.9
FILE_EXT=.tar.gz

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

EXTRACT_COMMAND=tar
EXTRACT_OPTIONS=vxzf
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

#export CMAKE_PREFIX_PATH=$PREFIX
#export PKG_CONFIG_PATH=$PREFIX/bin/pkg-config

CMAKE_GENERATOR="Unix Makefiles"

## ------------
## Start build.
## ------------

cd $DEST_PATH

echo "- extract."
$EXTRACT_COMMAND $EXTRACT_OPTIONS $FILE_PATH >> $LOG_PATH
cd $FILE_NAME

checkExitError
echo "- cmake."
cmake -G "$CMAKE_GENERATOR" >> $LOG_PATH

checkExitError
echo "- build."
$MAKE >> $LOG_PATH

checkExitError
echo "- install."
$MAKE install >> $LOG_PATH

if [ $AM64 -eq 0 ]; then
cp -r install/x86/mingw/bin/* $LBIN
cp -r install/x86/mingw/lib/* $LLIB
else
cp -r install/x64/mingw/bin/* $LBIN
cp -r install/x64/mingw/lib/* $LLIB
fi

cp -r install/include/* $LINC
mkdir -p $PREFIX/doc/$FILE_NAME
cp install/LICENSE $PREFIX/doc/$FILE_NAME/LICENSE
cp install/OpenCVConfig.cmake $PREFIX/doc/$FILE_NAME/OpenCVConfig.cmake
cp install/OpenCVConfig-version.cmake $PREFIX/doc/$FILE_NAME/OpenCVConfig-version.cmake

echo "- end! ($?)"
cd $WORKING_DIR
