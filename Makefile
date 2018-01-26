all: src/1-titik/titik.c src/2-garis/garis.cpp
	[ -d bin ] || mkdir -p bin
	gcc src/1-titik/titik.c -o bin/1
	g++ src/2-garis/garis.cpp -o bin/2

1: src/1-titik/titik.c
	[ -d bin ] || mkdir -p bin
	gcc src/1-titik/titik.c -o bin/1

2: src/2-garis/garis.cpp
	[ -d bin ] || mkdir -p bin
	g++ src/2-garis/garis.cpp -o bin/2

run:
	./bin/2

clean:
	rm -rf bin/*
	rm -rf bin