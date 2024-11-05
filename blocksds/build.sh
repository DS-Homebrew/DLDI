#!/bin/bash

set -e

rm -rf out
mkdir -p out

for i in source/platform/*; do
    FILE=`basename $i`
    if ! [[ $FILE == "00example" ]]; then
        make PLATFORM=$FILE
        cp $FILE.dldi out
    fi
done
