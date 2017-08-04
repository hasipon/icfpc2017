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

: > $1/.ginignore
echo "./common.hpp" >> $1/.ginignore
echo "./main.hpp"   >> $1/.ginignore

: > $1/Makefile
echo "a.out: main.cpp ai.hpp common.hpp" >> $1/Makefile
echo "	clang++ -std=c++14 main.cpp -o a.out" >> $1/Makefile
echo "main.cpp:" >> $1/Makefile
echo "	cp ../../common/hasi_template/main.cpp ./" >> $1/Makefile
echo "common.hpp:" >> $1/Makefile
echo "	cp ../../common/hasi_template/common.hpp ./" >> $1/Makefile

cp ../common/hasi_template/ai.hpp ./$1 
