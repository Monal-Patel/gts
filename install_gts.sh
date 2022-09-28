#!/bin/sh
./autogen.sh
./configure --prefix=$PWD/install
make install 
# need to add GTS_HOME/install/lib to $PATH and $LD_LIBRARY_PATH
