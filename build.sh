#!/bin/bash

if [ "$1" == "clean" ] || [ "$1" == "all" ]; then
    make clean -C shared
    make clean -C src/GLES_CM
    make clean -C demo
    make clean -C md2viewer
fi

if [ "$1" != "clean" ]; then
    make -j`nproc` -C shared
    make -j`nproc` -C src/GLES_CM
    make -C demo
    make -C md2viewer
fi 