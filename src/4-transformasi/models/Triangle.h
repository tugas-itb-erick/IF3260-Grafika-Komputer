#pragma once

#include "Point.h"
#include "Color.h"

class Triangle {
public:
  Point first;
  Point second;
  Point third;
  Color c;

  Triangle();
  Triangle(Point, Point, Point);
  Triangle(int, int, int, int, int, int);
  Triangle(Point, Point, Point, Color);
  Triangle(int, int, int, int, int, int, Color);
  Triangle(const Triangle&);
  Triangle& operator=(const Triangle&);
  Triangle operator+(const Point&);

  Point centroid();
  double area();
  bool hasPoint(Point); // check whether point is inside triangle
};