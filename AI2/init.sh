#!/bin/bash

function usage(){
    echo "usage: init.sh [AI-NAME]"
}
if [[ $1 = "" ]]; then
    usage
    exit 1
fi

echo new AI name: $1
mkdir $1

cat << EOS > $1/Makefile
a.out: main.cpp ai.hpp common.hpp
	clang++ -std=c++14 main.cpp -o a.out
main.cpp:
	cp ../../common/hasi_template2/main.cpp ./
common.hpp:
	cp ../../common/hasi_template2/common.hpp ./
EOS

cp ../common/hasi_template2/ai.hpp ./$1 
