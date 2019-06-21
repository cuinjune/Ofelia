#! /bin/bash
#
# this script automatically updates the sources for the PGMidi ios library
#
# to upgrade to a new version, change the version number below
#
# as long as the download link is formatted in the same way and folder
# structure are the same, this script should *just work*
#
# 2012 Dan Wilcox <danomatika@gmail.com> 
#

WD=$(dirname $0)

SRC=PGMidi
DEST=../libs/pgmidi

###

# move to this scripts dir
cd $WD

# get latest source
git clone git://github.com/petegoodliffe/PGMidi.git

# create destination dir
mkdir -p $DEST

# copy readme/license
cp -v $SRC/README.md $DEST

# copy sources
cp -v $SRC/Sources/PGMidi/* $DEST

# cleanup
rm -rf $SRC
