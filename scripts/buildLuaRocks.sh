#!/bin/bash

sudo apt install build-essential libreadline-dev unzip wget
wget https://luarocks.org/releases/luarocks-3.11.0.tar.gz
tar -zxpf luarocks-3.11.0.tar.gz
rm luarocks-3.11.0.tar.gz
cd luarocks-3.11.0
./configure
make
sudo make install
 mkdir ~/.luarocks