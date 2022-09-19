#!/bin/sh
./autogen.sh
./configure
make install --prefix=Location_of_install
sudo make install
