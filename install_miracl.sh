#!/bin/bash
mkdir miracl
cd miracl
wget https://codeload.github.com/miracl/MIRACL/zip/master -O MIRACL-master.zip
unzip -j -aa -L MIRACL-master.zip
echo "INFO: Compiling"
bash linux64
echo "INFO: Copy miracl.a to /usr/lib/libmiracl.a ..."
sudo cp miracl.a /usr/lib/libmiracl.a
sudo mkdir -p /usr/include/miracl
sudo cp *.h /usr/include/miracl
