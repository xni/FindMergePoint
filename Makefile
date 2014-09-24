collect-deps: src/main.cc Makefile
	g++ src/main.cc -o collect-deps -I./libgit2/include -L./libgit2-build -lgit2

all: collect-deps
