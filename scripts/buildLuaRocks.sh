#!/bin/bash

sudo apt install build-essential libreadline-dev unzip wget make -yq
wget "https://luarocks.org/releases/luarocks-3.12.2.tar.gz"
tar -zxpf luarocks-3.12.2.tar.gz
rm luarocks-3.12.2.tar.gz
cd luarocks-3.12.2
./configure
make
sudo make install
 mkdir ~/.luarocks
