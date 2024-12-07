#!/bin/bash

# fpp-media-extended-commands uninstall script
echo "Running fpp-mqtt-extended uninstall Script"

BASEDIR=$(dirname $0)
cd $BASEDIR
cd ..
make clean
