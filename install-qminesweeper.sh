#!/bin/bash

if [[ "$EUID" != "0" ]]; then
    echo "This script must be run as root"
    exit 1
fi

srcDir=/opt/GitHub/QMineSweeper
binDir=/opt/QtBuilds/QMineSweeper/Debug

ln -s $srcDir/qminesweeper.desktop /usr/share/applications/
ln -s $srcDir/qminesweeper.png /usr/share/pixmaps/
ln -s $srcDir/run-qminesweeper.sh /usr/bin/
ln -s $binDir/QMineSweeper /usr/bin/
