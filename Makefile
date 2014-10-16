collect-deps: src/main.cc Makefile
	g++ src/main.cc -std=c++11 -O2 -o collect-deps -I./libgit2/include -L./libgit2-build -lgit2

all: collect-deps
