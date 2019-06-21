#! /bin/bash

WD=$(dirname $0)

SRC=libpd
DEST=../libs/libpd

###

cd $WD

# get latest source
git clone git://github.com/libpd/libpd.git
cd $SRC
git submodule init
git submodule update
cd -

# remove uneeded makefiles, etc in src
find $SRC/pure-data -name "makefile*" -delete
find $SRC/pure-data -name "Makefile.am" -delete
find $SRC/pure-data -name "GNUmakefile.am" -delete
find $SRC/pure-data -name "*.pd" -delete
rm $SRC/pure-data/src/CHANGELOG.txt
rm $SRC/pure-data/src/notes.txt
rm $SRC/pure-data/src/pd.ico
rm $SRC/pure-data/src/pd.rc

# remove unneeded audio apis
rm $SRC/pure-data/src/s_audio_alsa*
rm $SRC/pure-data/src/s_audio_audiounit.c
rm $SRC/pure-data/src/s_audio_esd.c
rm $SRC/pure-data/src/s_audio_jack.c
rm $SRC/pure-data/src/s_audio_mmio.c
rm $SRC/pure-data/src/s_audio_oss.c
rm $SRC/pure-data/src/s_audio_pa.c
rm $SRC/pure-data/src/s_audio_paring.*

# remove unneeded midi apis
rm $SRC/pure-data/src/s_midi_alsa.c
rm $SRC/pure-data/src/s_midi_dummy.c
rm $SRC/pure-data/src/s_midi_mmio.c
rm $SRC/pure-data/src/s_midi_oss.c
rm $SRC/pure-data/src/s_midi_pm.c
rm $SRC/pure-data/src/s_midi.c

# remove uneeded fft library interfaces
rm $SRC/pure-data/src/d_fft_fftw.c

# remove some other stuff we don't need ...
rm $SRC/pure-data/src/s_entry.c
rm $SRC/pure-data/src/s_file.c
rm $SRC/pure-data/src/s_watchdog.c
rm $SRC/pure-data/src/u_pdreceive.c
rm $SRC/pure-data/src/u_pdsend.c
rm $SRC/pure-data/src/.dir-locals.el

# copy sources
mkdir -p $DEST/pure-data
cp -Rv $SRC/cpp $DEST/
cp -Rv $SRC/pure-data/src $DEST/pure-data
cp -Rv $SRC/pure-data/extra $DEST/pure-data
cp -Rv $SRC/libpd_wrapper $DEST/

# cleanup
rm -rf $SRC
