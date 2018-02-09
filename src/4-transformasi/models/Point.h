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
  void negate();
  bool operator==(const Point&);
  Point& operator=(const Point&);
  Point operator+(const Point&);
  Point operator-(const Point&);
  Point& operator*=(double);
  Point& operator+=(const Point&);
  Point& operator-=(const Point&);

  bool isDefined();
  bool isUndefined();

  static const Point UNDEF;
};