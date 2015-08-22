#!/bin/sh

CURDIR=$(pwd)
PATCHES=$(readlink -f $(dirname $0))/patches
CMDIR=$(readlink -f $PATCHES/../../../../)

for i in $(find "$PATCHES" -name *.patch -type f); do
    PATCHFILE=$(basename $i)
    PATCHDIR=$(dirname $i | sed "s#$PATCHES/##")
    if [ "$1" = "-r" ]; then
        echo "Resetting Git at $PATCHDIR"
        cd $CMDIR/$PATCHDIR
        git reset --hard || (echo "Reset failed" && exit)
    else
        echo "Applying $PATCHFILE to $PATCHDIR"
        patch -p1 -d $CMDIR/$PATCHDIR -i $i || (echo "Error while patching" && exit)
    fi
done

cd $CURDIR
