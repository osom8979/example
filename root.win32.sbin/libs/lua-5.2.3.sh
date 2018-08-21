#!/bin/sh

FILE_NAME=lua-5.2.3
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

## Convert Windows style directory to MSYS(UNIX) style directory.
## - Example: D:\\prefix\dir | D:\prefix\dir | D:/prefix/dir | /D/prefix/dir
PREFIX=`echo $PREFIX \
    | sed 's;\\\\\\\\;\\\\;g' \
    | sed 's;\\\\;/;g' \
    | sed 's;^\\([a-z,A-Z]\\):/;/\\1/;g'`

## ------------
## Start build.
## ------------

cd $DEST_PATH

echo "- extract."
$EXTRACT_COMMAND $EXTRACT_OPTIONS $FILE_PATH >> $LOG_PATH
cd $FILE_NAME

checkExitError
echo "- build."
$MAKE mingw >> $LOG_PATH

checkExitError
echo "- install."
$MAKE install INSTALL_TOP=$PREFIX >> $LOG_PATH

echo "- end! ($?)"
cd $WORKING_DIR
