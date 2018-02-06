#pragma once

#include "Color.h"

class Point {
public:
  int x;
  int y;

  Point();
  Point(int, int);
  Point(const Point&);
  Point& operator=(const Point&);
  Point operator+(const Point&);

  void translate(int tx, int ty, int a = 0, int b = 0);
  void dilate(double kx, double ky, int a = 0, int b = 0);
  void rotate(double theta, int a = 0, int b = 0);
};