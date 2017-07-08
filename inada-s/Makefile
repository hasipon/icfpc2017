all: build

.PHONY: build
build:
	mkdir -p bin
	clang++ src/main.cpp -O2 -lglog -o bin/main

.PHONY: format
format:
	bash format.sh

.PHONY: tag
tag:
	ctags -R --languages=c++
