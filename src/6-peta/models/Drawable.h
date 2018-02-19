#pragma once

#include <vector>
#include "Point.h"
#include "Triangle.h"
using namespace std;

class Drawable {
public:
  vector<Point> points;

  Drawable();
  Drawable(Point*);
  Drawable(vector<Point>);
  Drawable(const Drawable&);
  Drawable& operator=(const Drawable&);
  void useColor(Color);
  Point centroid();

  void translate(int tx, int ty, int a = 0, int b = 0);
  void dilate(double kx, double ky, int a = 0, int b = 0);
  void rotate(double theta, int a = 0, int b = 0);
};