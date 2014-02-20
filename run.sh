#!/bin/sh

OS=$(uname -s)
REL=$(uname -r)
MACHINE=$(uname -m)
SRCVER=-v1

build_run()
{
    local compiler_type CC version
    compiler_type=$1
    CC=$2
    static=$3

    case "$compiler_type" in
    clang)
        version=$($CC --version | head -n 1 | sed -n 's/^.*version \([0-9.]*\).*$/\1/p')
        ;;
    gcc)
        version=$($CC --version | head -n 1 | sed -n 's/^[^0-9]*\([0-9.]*\).*$/\1/p')
        ;;
    esac

    if [ -z "$version" ]; then
        version=unknown
    fi

    DIR=$OS/$REL/$MACHINE/$compiler_type/$version
    mkdir -p $DIR
    $CC $static -g -O0 -Wall -o $DIR/gen-core$static$SRCVER gen-core.c -lpthread
    (cd $DIR && ./gen-core$static$SRCVER)
}

for compiler in clang gcc; do
    if cc=$(which $compiler 2>/dev/null); then
        build_run $compiler $cc
        build_run $compiler $cc -static
    fi
done
