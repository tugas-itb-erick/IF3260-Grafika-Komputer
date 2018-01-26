#pragma once

#include "Point.h"
#include "Color.h"

class Line {
public:
  Point start;
  Point end;
  Color c;

  Line();
  Line(Point, Point);
  Line(int, int, int, int);
};