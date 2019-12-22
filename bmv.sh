#!/bin/sh

set -o posix

usage() {
    echo "usage: bmv [-C] -o RE -n replacement [file ...]"
    echo "  e.g.: to rename foo01.txt to bar01.txt, foo02.txt to bar02.txt " \
         "and so on:"
    echo "        $ bmv -o 'foo([[:digit:]]+).*' -n 'bar\1.txt' *.txt"

}

args=$(getopt Co:n: $*)
if [ $? -ne 0 ]
then
    usage
fi

# indicates an end to option processing
set -- $args

while [ $# -ne 0 ]
do
    case "$1"
    in
        -C)
            opt_C=1;
            shift;;
        -o)
            opt_o=1;
            optarg_o="$2";
            shift; shift;;
        -n)
            opt_n=1;
            optarg_n="$2";
            shift; shift;;
        --)
            shift;
            break;;
    esac
done

if [ ! $opt_o -o ! $opt_n ]
then
    usage
    exit 1
fi

if [ $opt_C ]
then
    echo "check, does not modify anything"
fi

for name_o in $*; do
    name_n=$(echo ${name_o} | sed -E "s/${optarg_o}/${optarg_n}/g")
    echo "$name_o -> $name_n"

    if [ ! $opt_C ]
    then
        mv $name_o $name_n
    fi
done
