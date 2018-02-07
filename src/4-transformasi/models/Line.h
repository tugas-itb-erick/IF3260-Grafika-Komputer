#pragma once

#include "Point.h"
#include "Color.h"

class Line {
public:
  Point first;
  Point second;
  Color c;
  static const double UNDEF;

  Line();
  Line(Point, Point);
  Line(int, int, int, int);
  Line(Point, Point, Color);
  Line(int, int, int, int, Color);
  Line(const Line&);
  Line& operator=(const Line&);

  double gradient() const;
};