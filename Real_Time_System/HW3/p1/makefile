all: build run

build: main.cpp
	c++ main.cpp --std=c++17 -lrt -lpthread -o main

run: build
	./main

clear:
	rm -f main
