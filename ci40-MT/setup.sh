#!/bin/sh -V
#install gcc cross compiler


echo "Downloading mips linux compiler"

wget https://sourcery.mentor.com/GNUToolchain/package14486/public/mips-linux-gnu/mips-2016.05-8-mips-linux-gnu-i686-pc-linux-gnu.tar.bz2

echo "Extracting mips-linux-gnu-gcc"

tar -xvf mips-2016.05-8-mips-linux-gnu-i686-pc-linux-gnu.tar.bz2 
