#!/bin/sh

if [ -z $1 ]; then
    echo "[PARAM1] Unknown file name error."
    exit 1
elif [ -z $2 ]; then
    echo "[PARAM2] Unknown file extension error."
    exit 1
fi

FILE_NAME=$1
FILE_EXT=$2
CONFIG_EXT=$3

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

EXTRACT_COMMAND=
EXTRACT_OPTIONS=
MAKE=

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

if [ "$FILE_EXT" == ".tar.gz" ]; then
    EXTRACT_COMMAND=tar
    EXTRACT_OPTIONS=vxzf
elif [ "$FILE_EXT" == ".tgz" ]; then
    EXTRACT_COMMAND=tar
    EXTRACT_OPTIONS=vxzf
elif [ "$FILE_EXT" == ".tar.bz2" ]; then
    EXTRACT_COMMAND=tar
    EXTRACT_OPTIONS=vxjf
elif [ "$FILE_EXT" == ".zip" ]; then
    EXTRACT_COMMAND=unzip
    EXTRACT_OPTIONS=-o
fi

if [ -z $MAKE ]; then
    MAKE=make
fi

if [ ! -d $TEMP_DIR ]; then
    mkdir -p "$TEMP_DIR"
fi

## libtool: link: only absolute run-paths are allowed
##
## Convert Windows style directory to MSYS(UNIX) style directory.
## - Example: D:\\prefix\dir | D:\prefix\dir | D:/prefix/dir | /D/prefix/dir
PREFIX=`echo $PREFIX \
    | sed 's;\\\\\\\\;\\\\;g' \
    | sed 's;\\\\;/;g' \
    | sed 's;^\\([a-z,A-Z]\\):/;/\\1/;g'`

CFGFLAGS="--prefix=$PREFIX $CONFIG_EXT"
#CFGFLAGS="$CONFIG_EXT"

## ------------
## Start build.
## ------------

cd $DEST_PATH

echo "- extract."
$EXTRACT_COMMAND $EXTRACT_OPTIONS $FILE_PATH >> $LOG_PATH
cd $FILE_NAME

checkExitError
echo "- configure."
./configure $CFGFLAGS >> $LOG_PATH

checkExitError
echo "- build."
$MAKE >> $LOG_PATH

checkExitError
echo "- install."
$MAKE install >> $LOG_PATH

echo "- end! ($?)"
cd $WORKING_DIR
