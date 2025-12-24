export C_COMPILER_PACKAGE=""
export CXX_COMPILER_PACKAGE=""
export C_COMPILER=""
export CXX_COMPILER=""

if [[ $COMPILER = "gcc" ]]; then
	export C_COMPILER_PACKAGE="gcc"
	export CXX_COMPILER_PACKAGE="g++"
	export C_COMPILER="/usr/bin/gcc"
	export CXX_COMPILER="/usr/bin/g++"
elif [[ $COMPILER = "clang" ]]; then
	case $(uname -v) in
    *FreeBSD*)
		export C_COMPILER_PACKAGE="llvm"
		export CXX_COMPILER_PACKAGE=""
        ;;
    *)
    	export C_COMPILER_PACKAGE="clang"
		export CXX_COMPILER_PACKAGE=""
        ;;
	esac

	export C_COMPILER="/usr/bin/clang"
	export CXX_COMPILER="/usr/bin/clang++"
fi
