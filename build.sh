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
    if [[ $FILE -eq "ttio" ]]; then
        make -C $FILE clean
        make -C $FILE SDHC=1 clean
        make -C $FILE
        cp $FILE/*.dldi $OUT
        make -C $FILE clean
        make -C $FILE SDHC=1
        cp $FILE/*.dldi $OUT
    else
        make -C $FILE clean
        make -C $FILE
        cp $FILE/*.dldi $OUT
    fi
done
cd $TOPDIR

cp -r $TOPDIR/prebuilts/*.dldi $OUT
