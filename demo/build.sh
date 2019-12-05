#!/bin/bash

if [ "$1" == "clean" ] || [ "$1" == "all" ]; then
    make clean
    make clean -C ../shared/
    make clean -C ../src/EGL
    make clean -C ../src/GLES_CM
fi

if [ "$1" != "clean" ]; then
    make -j`nproc` -C ../shared/
    make -j`nproc` -C ../src/EGL
    make -j`nproc` -C ../src/GLES_CM
    make
fi 