#!/bin/sh

FILE_NAME=tinyxml-2.6.2
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

## ------------
## Start build.
## ------------

cd $DEST_PATH

echo "- extract."
$EXTRACT_COMMAND $EXTRACT_OPTIONS $FILE_PATH >> $LOG_PATH
cd $FILE_NAME

checkExitError
echo "- patch."
patch -p1 < $WORKING_DIR/$FILE_NAME.patch >> $LOG_PATH

checkExitError
echo "- build."
$MAKE all >> $LOG_PATH

checkExitError
echo "- install."
cp *.h $LINC
cp *.a $LLIB
mkdir -p $PREFIX/doc/$FILE_NAME
cp -r docs/* $PREFIX/doc/$FILE_NAME

echo "- end! ($?)"
cd $WORKING_DIR
