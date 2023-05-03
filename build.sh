#!/bin/bash

# Clean out dir before starting

TOPDIR=$PWD
OUT=$TOPDIR/out

# Clear output directory before starting
rm -rf $OUT
mkdir $OUT

# OSS drivers first
cd source
for FILE in *; do
    make -C $FILE clean
    make -C $FILE
    cp $FILE/*.dldi $OUT
done
cd $TOPDIR

cp -r $TOPDIR/prebuilts/*.dldi $OUT
