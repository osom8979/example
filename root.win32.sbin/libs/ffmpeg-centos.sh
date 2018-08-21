#!/bin/sh

## Compile FFmpeg on CentOS
## https://trac.ffmpeg.org/wiki/CompilationGuide/Centos

SOURCE_DIR=$HOME/Project/ffmpeg_sources
BUILD_DIR=$HOME/Project/ffmpeg_build
BIN_DIR=$HOME/bin

mkdir -p $SOURCE_DIR
mkdir -p $BUILD_DIR
mkdir -p $BIN_DIR

## Yasm
cd $SOURCE_DIR
git clone --depth 1 git://github.com/yasm/yasm.git
cd yasm
autoreconf -fiv
./configure --prefix="$BUILD_DIR" --bindir="$BIN_DIR"
make
make install
make distclean

## libx264
cd $SOURCE_DIR
git clone --depth 1 git://git.videolan.org/x264
cd x264
./configure --prefix="$BUILD_DIR" --bindir="$BIN_DIR" --enable-static
make
make install
make distclean

## libx265
cd $SOURCE_DIR
hg clone https://bitbucket.org/multicoreware/x265
cd $SOURCE_DIR/x265/build/linux
cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="$BUILD_DIR" -DENABLE_SHARED:bool=off ../../source
make
make install

## libfdk_aac
cd $SOURCE_DIR
git clone --depth 1 git://git.code.sf.net/p/opencore-amr/fdk-aac
cd fdk-aac
autoreconf -fiv
./configure --prefix="$BUILD_DIR" --disable-shared
make
make install
make distclean

## libmp3lame
cd $SOURCE_DIR
curl -L -O http://downloads.sourceforge.net/project/lame/lame/3.99/lame-3.99.5.tar.gz
tar xzvf lame-3.99.5.tar.gz
cd lame-3.99.5
./configure --prefix="$BUILD_DIR" --bindir="$BIN_DIR" --disable-shared --enable-nasm
make
make install
make distclean

## libopus
cd $SOURCE_DIR
git clone git://git.opus-codec.org/opus.git
cd opus
autoreconf -fiv
./configure --prefix="$BUILD_DIR" --disable-shared
make
make install
make distclean

## libogg
cd $SOURCE_DIR
curl -O http://downloads.xiph.org/releases/ogg/libogg-1.3.2.tar.gz
tar xzvf libogg-1.3.2.tar.gz
cd libogg-1.3.2
./configure --prefix="$BUILD_DIR" --disable-shared
make
make install
make distclean

## libvorbis
cd $SOURCE_DIR
curl -O http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.4.tar.gz
tar xzvf libvorbis-1.3.4.tar.gz
cd libvorbis-1.3.4
LDFLAGS="-L$BUILD_DIR/lib" CPPFLAGS="-I$BUILD_DIR/include" ./configure --prefix="$BUILD_DIR" --with-ogg="$BUILD_DIR" --disable-shared
make
make install
make distclean

## libvpx
cd $SOURCE_DIR
git clone --depth 1 https://chromium.googlesource.com/webm/libvpx.git
cd libvpx
./configure --prefix="$BUILD_DIR" --disable-examples
make
make install
make clean

## FFmpeg
cd $SOURCE_DIR
git clone --depth 1 git://source.ffmpeg.org/ffmpeg
cd ffmpeg
PKG_CONFIG_PATH="$BUILD_DIR/lib/pkgconfig" ./configure --prefix="$BUILD_DIR" --extra-cflags="-I$BUILD_DIR/include" --extra-ldflags="-L$BUILD_DIR/lib" --bindir="$BIN_DIR" --pkg-config-flags="--static" --enable-gpl --enable-nonfree --enable-libfdk-aac --enable-libfreetype --enable-libmp3lame --enable-libopus --enable-libvorbis --enable-libvpx --enable-libx264 --enable-libx265 --enable-shared --enable-static
make -j8
make install
make distclean
hash -r

