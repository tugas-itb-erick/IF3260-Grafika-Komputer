all: src/1-titik/main.c src/2-garis/main.cpp src/2-garis/models/Color.cpp src/2-garis/models/Point.cpp src/2-garis/models/Line.cpp src/2-garis/models/CharDrawable.cpp src/3-bidang/main.cpp src/3-bidang/models/Color.cpp src/3-bidang/models/Point.cpp src/3-bidang/models/Line.cpp src/3-bidang/models/Triangle.cpp src/3-bidang/models/CharDrawable.cpp
	[ -d bin ] || mkdir -p bin
	gcc src/1-titik/main.c -o bin/1
	g++ src/2-garis/main.cpp src/2-garis/models/Color.cpp src/2-garis/models/Point.cpp src/2-garis/models/Line.cpp src/2-garis/models/CharDrawable.cpp -o bin/2 -std=c++11
	g++ src/3-bidang/main.cpp src/3-bidang/models/Color.cpp src/3-bidang/models/Point.cpp src/3-bidang/models/Line.cpp src/3-bidang/models/Triangle.cpp src/3-bidang/models/CharDrawable.cpp -o bin/3 -std=c++11

1: src/1-titik/main.c
	[ -d bin ] || mkdir -p bin
	gcc src/1-titik/main.c -o bin/1

2: src/2-garis/main.cpp src/2-garis/models/Color.cpp src/2-garis/models/Point.cpp src/2-garis/models/Line.cpp src/2-garis/models/CharDrawable.cpp
	[ -d bin ] || mkdir -p bin
	g++ src/2-garis/main.cpp src/2-garis/models/Color.cpp src/2-garis/models/Point.cpp src/2-garis/models/Line.cpp src/2-garis/models/CharDrawable.cpp -o bin/2 -std=c++11

3: src/3-bidang/main.cpp src/3-bidang/models/Color.cpp src/3-bidang/models/Point.cpp src/3-bidang/models/Line.cpp src/3-bidang/models/Triangle.cpp src/3-bidang/models/CharDrawable.cpp
	[ -d bin ] || mkdir -p bin
	g++ src/3-bidang/main.cpp src/3-bidang/models/Color.cpp src/3-bidang/models/Point.cpp src/3-bidang/models/Line.cpp src/3-bidang/models/Triangle.cpp src/3-bidang/models/CharDrawable.cpp -o bin/3 -std=c++11

run:
	clear
	./bin/3

clean:
	rm -rf bin/*
	rm -rf bin