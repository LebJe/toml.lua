export C_COMPILER_PACKAGE=""
export CXX_COMPILER_PACKAGE=""
export C_COMPILER=""
export CXX_COMPILER=""

if [[ $COMPILER = "gcc" ]]; then
	export C_COMPILER_PACKAGE="gcc"
	case $(uname -v) in
    *FreeBSD*)
		export CXX_COMPILER_PACKAGE=""
        ;;
    *)
    export CXX_COMPILER_PACKAGE="g++"
        ;;
	esac
	
	export C_COMPILER="$(which gcc)"
	export CXX_COMPILER="$(which g++)"
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

	export C_COMPILER="$(which clang)"
	export CXX_COMPILER="$(which clang++)"
fi
