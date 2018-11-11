#! /bin/bash

WD=$(dirname $0)

VER=5.3.4

SRC=lua-$VER
DEST=../libs/lua

###

cd $WD

# get latest source
curl -O http://www.lua.org/ftp/$SRC.tar.gz
tar -xvf $SRC.tar.gz

# remove lua standalone console and compiler sources
rm -v $SRC/src/lua.c $SRC/src/luac.c

# create dir
mkdir -p $DEST

# copy license
cp -v $SRC/COPYRIGHT $DEST

# copy sources
cp -v $SRC/src/*.h $DEST
cp -v $SRC/src/*.c $DEST
cp -v $SRC/etc/*.hpp $DEST

# cleanup
rm -rfv lua*

