
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
case $(uname -v) in
    *FreeBSD*)
		./configure --with-lua-bin="/usr/local/bin/"
        ;;
    *)
		./configure
        ;;
	esac

make
sudo make install
mkdir ~/.luarocks
