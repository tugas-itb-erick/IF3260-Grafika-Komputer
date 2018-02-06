#pragma once

#include <vector>
#include "Point.h"
#include "Triangle.h"
using namespace std;

class Drawable {
public:
  vector<Point> points;
  vector<Triangle> triangles;

  Drawable();
  Drawable(Point*, Triangle*);
  Drawable(vector<Point>, vector<Triangle>);
  Drawable(const Drawable&);
  Drawable& operator=(const Drawable&);
  void useColor(Color);
  Point centroid();
};