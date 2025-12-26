# First argument is the Lua version Luarocks should use: 5.4, 5.3, 5.2, 5.1

case $(uname -v) in
    *FreeBSD*)
		sudo pkg install -y unzip wget gmake readline curl
        ;;
    *)
		sudo apt install build-essential libreadline-dev unzip wget make -yq
        ;;
	esac

wget "https://luarocks.org/releases/luarocks-3.12.2.tar.gz"
tar -zxpf luarocks-3.12.2.tar.gz
rm luarocks-3.12.2.tar.gz
cd luarocks-3.12.2

case $1 in
    *5.4*)
		./configure --lua-version="5.4"
        ;;
    *5.3*)
		./configure --lua-version="5.3"
        ;;
    *5.2*)
		./configure --lua-version="5.2"
        ;;
    *5.1*)
		./configure --lua-version="5.1"
        ;;
    *)
		./configure
        ;;
esac

make
sudo make install
mkdir ~/.luarocks
