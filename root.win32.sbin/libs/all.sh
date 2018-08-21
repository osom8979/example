#!/bin/sh
# WARNING: Don't use GnuWin32 PATH.

## Force PREFIX setting.
if [ ! -z $1 ]; then
    PREFIX=$1
fi

function runScript {
    local SCRIPT_NAME=$1

    echo
    echo "Run $SCRIPT_NAME"
    ./$SCRIPT_NAME
}

echo "[BUILD ALL LIBRARIES]"
echo "- PREFIX=$PREFIX"
echo


### ------------------------------------
### The calling sequence is important!!!
### ------------------------------------

# BUILD.
runScript "yasm-1.3.0.sh"
runScript "cmake-3.0.2.sh"

# GENERAL.
#runScript "bzip2-1.0.6.sh"
#runScript "unzip-6.0.sh"
#runScript "pthreads-w32-2-9-1-release.sh"
runScript "libiconv-1.14.sh"

# UTILITY.
runScript "gmock-1.7.0.sh"
runScript "stx-btree-0.9.sh"
runScript "tinyxml-2.6.2.sh"
runScript "expat-2.1.0.sh"
runScript "sqlite-autoconf-3080600.sh"
runScript "lua-5.2.3.sh"

# COMPRESS & IMAGE & SOUND.
runScript "zlib-1.2.8.sh"
runScript "libpng-1.6.12.sh"
runScript "jpeg-9a.sh"
runScript "libogg-1.3.2.sh"
runScript "libvorbis-1.3.4.sh"

# GRAPHICS.
runScript "freetype-2.5.4.sh"
runScript "SDL-1.2.15.sh"
#runScript "SDL2-2.0.3.sh"
#runScript "SDL-9f8a2b28f61d.sh"
#runScript "SDL2_ttf-2.0.12.sh"
#runScript "SDL2_image-2.0.0.sh"
#runScript "SDL2_mixer-2.0.0.sh"
runScript "irrlicht-1.8.1.sh"

# MATHEMATICS.
runScript "fftw-3.3.4.sh"
runScript "libsvm-3.19.sh"

# MULTIMEDIA.
runScript "x264-snapshot-20140929-2245.sh"
runScript "opencv-2.4.9.sh"
# Don't use the msysGit.
#runScript "ffmpeg-2.4.1.sh"

# FRAMEWORK.
#runScript "wxWidgets-2.8.12.sh"
runScript "wxWidgets-3.0.1.sh"
