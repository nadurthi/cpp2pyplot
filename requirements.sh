#!/bin/bash

export WORKDIR=$(pwd)

mkdir tmp
cd tmp


git clone https://github.com/google/benchmark.git
cd benchmark
# If you want to build tests and don't use BENCHMARK_DOWNLOAD_DEPENDENCIES, then
git clone https://github.com/google/googletest.git
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=RELEASE
make

sudo make install





#---------------------
cd $WORKDIR
# sudo rm -r tmp