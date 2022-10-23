#!/bin/sh
./autogen.sh
./configure --prefix=$PWD/install
make install 

# need to add GTS_HOME/install/lib to $PATH and $LD_LIBRARY_PATH
file=$HOME/.bashrc
if [ -f "$file" ] 
then
	echo "export GTS_HOME=$PWD" >> $file
	echo 'export LD_LIBRARY_PATH=$GTS_HOME/install/lib:$LD_LIBRARY_PATH' >> $file
	echo 'export PATH=$GTS_HOME/install/lib:$PATH' >> $file
	source ~/.bashrc
else
	echo "$file not found"
	exit 2
fi
