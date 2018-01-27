#pragma once

#include "Point.h"
#include "Color.h"

class Line {
public:
  Point start;
  Point end;
  Color c;
  static const double UNDEF;

  Line();
  Line(Point, Point);
  Line(int, int, int, int);
  Line(Point, Point, const Color&);
  Line(int, int, int, int, const Color&);
  Line(const Line&);
  Line& operator=(const Line&);

  double gradient();
};