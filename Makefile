all: src/1-titik/main.c src/2-garis/main.cpp src/2-garis/models/Color.cpp src/2-garis/models/Point.cpp src/2-garis/models/Line.cpp src/2-garis/models/CharDrawable.cpp src/3-bidang/main.cpp src/3-bidang/models/Color.cpp src/3-bidang/models/Point.cpp src/3-bidang/models/Line.cpp src/3-bidang/models/Triangle.cpp src/3-bidang/models/CharDrawable.cpp src/4-transformasi/main.cpp src/4-transformasi/models/Color.cpp src/4-transformasi/models/Point.cpp src/4-transformasi/models/Line.cpp src/4-transformasi/models/Triangle.cpp src/4-transformasi/models/Drawable.cpp src/4-transformasi/models/Buffer.cpp src/5-window/main.cpp src/5-window/models/Color.cpp src/5-window/models/Point.cpp src/5-window/models/Line.cpp src/5-window/models/Triangle.cpp src/5-window/models/Drawable.cpp src/5-window/models/Buffer.cpp src/6-peta/main.cpp src/6-peta/models/Color.cpp src/6-peta/models/Point.cpp src/6-peta/models/Line.cpp src/6-peta/models/Triangle.cpp src/6-peta/models/Drawable.cpp src/6-peta/models/Buffer.cpp src/7-mouse/main.cpp src/7-mouse/models/Color.cpp src/7-mouse/models/Point.cpp src/7-mouse/models/Line.cpp src/7-mouse/models/Triangle.cpp src/7-mouse/models/Drawable.cpp src/7-mouse/models/Buffer.cpp src/nub/main.cpp src/nub/models/Color.cpp src/nub/models/Point.cpp src/nub/models/Line.cpp src/nub/models/Triangle.cpp src/nub/models/Drawable.cpp src/nub/models/Buffer.cpp
	[ -d bin ] || mkdir -p bin
	gcc src/1-titik/main.c -o bin/1
	g++ src/2-garis/main.cpp src/2-garis/models/Color.cpp src/2-garis/models/Point.cpp src/2-garis/models/Line.cpp src/2-garis/models/CharDrawable.cpp -o bin/2 -std=c++11
	g++ src/3-bidang/main.cpp src/3-bidang/models/Color.cpp src/3-bidang/models/Point.cpp src/3-bidang/models/Line.cpp src/3-bidang/models/Triangle.cpp src/3-bidang/models/CharDrawable.cpp -o bin/3 -std=c++11
	g++ src/4-transformasi/main.cpp src/4-transformasi/models/Color.cpp src/4-transformasi/models/Point.cpp src/4-transformasi/models/Line.cpp src/4-transformasi/models/Triangle.cpp src/4-transformasi/models/Drawable.cpp src/4-transformasi/models/Buffer.cpp -o bin/4 -std=c++11
	g++ src/5-window/main2.cpp src/5-window/models/Color.cpp src/5-window/models/Point.cpp src/5-window/models/Line.cpp src/5-window/models/Triangle.cpp src/5-window/models/Drawable.cpp src/5-window/models/Buffer.cpp -o bin/5 -std=c++11
	g++ src/6-peta/main2.cpp src/6-peta/models/Color.cpp src/6-peta/models/Point.cpp src/6-peta/models/Line.cpp src/6-peta/models/Triangle.cpp src/6-peta/models/Drawable.cpp src/6-peta/models/Buffer.cpp -o bin/6 -std=c++11
	g++ src/7-mouse/main.cpp src/7-mouse/models/Color.cpp src/7-mouse/models/Point.cpp src/7-mouse/models/Line.cpp src/7-mouse/models/Triangle.cpp src/7-mouse/models/Drawable.cpp src/7-mouse/models/Buffer.cpp -o bin/7 -std=c++11
	g++ src/nub/main.cpp src/nub/models/Color.cpp src/nub/models/Point.cpp src/nub/models/Line.cpp src/nub/models/Triangle.cpp src/nub/models/Drawable.cpp src/nub/models/Buffer.cpp -o bin/nub -std=c++11

1: src/1-titik/main.c
	[ -d bin ] || mkdir -p bin
	gcc src/1-titik/main.c -o bin/1

2: src/2-garis/main.cpp src/2-garis/models/Color.cpp src/2-garis/models/Point.cpp src/2-garis/models/Line.cpp src/2-garis/models/CharDrawable.cpp
	[ -d bin ] || mkdir -p bin
	g++ src/2-garis/main.cpp src/2-garis/models/Color.cpp src/2-garis/models/Point.cpp src/2-garis/models/Line.cpp src/2-garis/models/CharDrawable.cpp -o bin/2 -std=c++11

3: src/3-bidang/main.cpp src/3-bidang/models/Color.cpp src/3-bidang/models/Point.cpp src/3-bidang/models/Line.cpp src/3-bidang/models/Triangle.cpp src/3-bidang/models/CharDrawable.cpp
	[ -d bin ] || mkdir -p bin
	g++ src/3-bidang/main.cpp src/3-bidang/models/Color.cpp src/3-bidang/models/Point.cpp src/3-bidang/models/Line.cpp src/3-bidang/models/Triangle.cpp src/3-bidang/models/CharDrawable.cpp -o bin/3 -std=c++11

4: src/4-transformasi/main.cpp src/4-transformasi/models/Color.cpp src/4-transformasi/models/Point.cpp src/4-transformasi/models/Line.cpp src/4-transformasi/models/Triangle.cpp src/4-transformasi/models/Drawable.cpp src/4-transformasi/models/Buffer.cpp
	[ -d bin ] || mkdir -p bin
	g++ src/4-transformasi/main.cpp src/4-transformasi/models/Color.cpp src/4-transformasi/models/Point.cpp src/4-transformasi/models/Line.cpp src/4-transformasi/models/Triangle.cpp src/4-transformasi/models/Drawable.cpp src/4-transformasi/models/Buffer.cpp -o bin/4 -std=c++11

5: src/5-window/main.cpp src/5-window/models/Color.cpp src/5-window/models/Point.cpp src/5-window/models/Line.cpp src/5-window/models/Triangle.cpp src/5-window/models/Drawable.cpp src/5-window/models/Buffer.cpp
	[ -d bin ] || mkdir -p bin
	g++ src/5-window/main.cpp src/5-window/models/Color.cpp src/5-window/models/Point.cpp src/5-window/models/Line.cpp src/5-window/models/Triangle.cpp src/5-window/models/Drawable.cpp src/5-window/models/Buffer.cpp -o bin/5 -std=c++11

52: src/5-window/main2.cpp src/5-window/models/Color.cpp src/5-window/models/Point.cpp src/5-window/models/Line.cpp src/5-window/models/Triangle.cpp src/5-window/models/Drawable.cpp src/5-window/models/Buffer.cpp
	[ -d bin ] || mkdir -p bin
	g++ src/5-window/main2.cpp src/5-window/models/Color.cpp src/5-window/models/Point.cpp src/5-window/models/Line.cpp src/5-window/models/Triangle.cpp src/5-window/models/Drawable.cpp src/5-window/models/Buffer.cpp -o bin/52 -std=c++11
 
6: src/6-peta/main.cpp src/6-peta/models/Color.cpp src/6-peta/models/Point.cpp src/6-peta/models/Line.cpp src/6-peta/models/Triangle.cpp src/6-peta/models/Drawable.cpp src/6-peta/models/Buffer.cpp
	[ -d bin ] || mkdir -p bin
	g++ src/6-peta/main.cpp src/6-peta/models/Color.cpp src/6-peta/models/Point.cpp src/6-peta/models/Line.cpp src/6-peta/models/Triangle.cpp src/6-peta/models/Drawable.cpp src/6-peta/models/Buffer.cpp -o bin/6 -std=c++11

62: src/6-peta/main2.cpp src/6-peta/models/Color.cpp src/6-peta/models/Point.cpp src/6-peta/models/Line.cpp src/6-peta/models/Triangle.cpp src/6-peta/models/Drawable.cpp src/6-peta/models/Buffer.cpp
	[ -d bin ] || mkdir -p bin
	g++ src/6-peta/main2.cpp src/6-peta/models/Color.cpp src/6-peta/models/Point.cpp src/6-peta/models/Line.cpp src/6-peta/models/Triangle.cpp src/6-peta/models/Drawable.cpp src/6-peta/models/Buffer.cpp -o bin/62 -std=c++11

7: src/7-mouse/main.cpp src/7-mouse/models/Color.cpp src/7-mouse/models/Point.cpp src/7-mouse/models/Line.cpp src/7-mouse/models/Triangle.cpp src/7-mouse/models/Drawable.cpp src/7-mouse/models/Buffer.cpp
	[ -d bin ] || mkdir -p bin
	g++ src/7-mouse/main.cpp src/7-mouse/models/Color.cpp src/7-mouse/models/Point.cpp src/7-mouse/models/Line.cpp src/7-mouse/models/Triangle.cpp src/7-mouse/models/Drawable.cpp src/7-mouse/models/Buffer.cpp -o bin/7 -std=c++11

8: src/8-uts/main.cpp src/8-uts/models/Color.cpp src/8-uts/models/Point.cpp src/8-uts/models/Line.cpp src/8-uts/models/Triangle.cpp src/8-uts/models/Drawable.cpp src/8-uts/models/Buffer.cpp src/8-uts/models/CharDrawable.cpp
	[ -d bin ] || mkdir -p bin
	g++ src/8-uts/main.cpp src/8-uts/models/Color.cpp src/8-uts/models/Point.cpp src/8-uts/models/Line.cpp src/8-uts/models/Triangle.cpp src/8-uts/models/Drawable.cpp src/8-uts/models/Buffer.cpp src/8-uts/models/CharDrawable.cpp -o bin/8 -std=c++11

nub: src/nub/main.cpp src/nub/models/Color.cpp src/nub/models/Point.cpp src/nub/models/Line.cpp src/nub/models/Triangle.cpp src/nub/models/Drawable.cpp src/nub/models/Buffer.cpp
	[ -d bin ] || mkdir -p bin
	g++ src/nub/main.cpp src/nub/models/Color.cpp src/nub/models/Point.cpp src/nub/models/Line.cpp src/nub/models/Triangle.cpp src/nub/models/Drawable.cpp src/nub/models/Buffer.cpp -o bin/nub -std=c++11

run:
	clear
	./bin/8

clean:
	rm -rf bin/*
	rm -rf bin
