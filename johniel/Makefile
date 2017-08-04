CXX=clang++-3.8
CXX_FLAGS=-O2 -std=c++14
OUTPUT=./punter

all:
	$(CXX) $(CXX_FLAGS) -I ./lib/cmdline -I ../../common ./src/main.cpp -o $(OUTPUT)

clean:
	rm ./punter
