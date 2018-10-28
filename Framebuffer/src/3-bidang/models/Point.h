#pragma once

#include "Color.h"

class Point {
public:
  int x;
  int y;
  Color c;

  Point();
  Point(int, int);
  Point(int, int, Color);
  Point(const Point&);
  Point& operator=(const Point&);
  Point operator+(const Point&);
};