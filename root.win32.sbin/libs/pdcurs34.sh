#!/bin/sh

FILE_NAME=pdcurs34
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

## ------------
## Start build.
## ------------

mkdir -p $DEST_PATH/$FILE_NAME
cd $DEST_PATH/$FILE_NAME

echo "- extract."
$EXTRACT_COMMAND $EXTRACT_OPTIONS $FILE_PATH >> $LOG_PATH
cd win32

checkExitError
echo "- patch."
patch -l -p2 < $WORKING_DIR/$FILE_NAME.patch >> $LOG_PATH

checkExitError
echo "- build."
$MAKE -f mingwin32.mak libs >> $LOG_PATH

checkExitError
echo "- install."
cp -r ../*.h $LINC
cp panel.a $LLIB/libpanel.a

echo "- end! ($?)"
cd $WORKING_DIR
