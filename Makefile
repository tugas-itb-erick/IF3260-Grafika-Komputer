all: src/1-titik/main.c src/2-garis/main.cpp src/2-garis/models/Color.cpp src/2-garis/models/Point.cpp src/2-garis/models/Line.cpp src/2-garis/models/CharDrawable.cpp
	[ -d bin ] || mkdir -p bin
	gcc src/1-titik/main.c -o bin/1
	g++ src/2-garis/main.cpp src/2-garis/models/Color.cpp src/2-garis/models/Point.cpp src/2-garis/models/Line.cpp src/2-garis/models/CharDrawable.cpp -o bin/2 -std=c++11

1: src/1-titik/main.c
	[ -d bin ] || mkdir -p bin
	gcc src/1-titik/main.c -o bin/1

2: src/2-garis/main.cpp src/2-garis/models/Color.cpp src/2-garis/models/Point.cpp src/2-garis/models/Line.cpp src/2-garis/models/CharDrawable.cpp
	[ -d bin ] || mkdir -p bin
	g++ src/2-garis/main.cpp src/2-garis/models/Color.cpp src/2-garis/models/Point.cpp src/2-garis/models/Line.cpp src/2-garis/models/CharDrawable.cpp -o bin/2 -std=c++11

run:
	./bin/2

clean:
	rm -rf bin/*
	rm -rf bin