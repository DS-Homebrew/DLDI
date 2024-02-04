#!/bin/bash

# Clean out dir before starting

TOPDIR=$PWD
OUT=$TOPDIR/out

# Clear output directory before starting
rm -rf $OUT
mkdir $OUT

set -e

# OSS drivers first
cd source
for FILE in *; do
    if [[ $FILE == "scdssdhc2" ]]; then
        make -C $FILE clean
        make -C $FILE -f Makefile_ttio clean
        make -C $FILE
        cp $FILE/*.dldi $OUT
        make -C $FILE clean
        make -C $FILE -f Makefile_ttio
        cp $FILE/*.dldi $OUT
    elif [[ $FILE == "rpg_sd" ]]; then
        # Clean before starting
        make -C $FILE clean
        make -C $FILE -f Makefile_ak2 clean
        make -C $FILE -f Makefile_ak2_singlewrite clean
        make -C $FILE -f Makefile_r4idsn clean

        # Build everything
        make -C $FILE
        make -C $FILE -f Makefile_ak2
        make -C $FILE -f Makefile_ak2_singlewrite
        make -C $FILE -f Makefile_r4idsn

        # Copy to out
        cp $FILE/*.dldi $OUT
    else
        make -C $FILE clean
        make -C $FILE
        cp $FILE/*.dldi $OUT
    fi
done
cd $TOPDIR

cp -r $TOPDIR/prebuilts/*.dldi $OUT
