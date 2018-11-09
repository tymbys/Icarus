#!/bin/sh

#rm -rf build/
mkdir build
mkdir rootfs 

git clone https://github.com/mattgodbolt/seasocks.git

cd build
mkdir seasock
cd seasock
#cmake -DCMAKE_INSTALL_PREFIX=../../rootfs/  ../../seasocks/
cmake ../../seasocks/

make -j4
sudo make install
sudo ldconfig

