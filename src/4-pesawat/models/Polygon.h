#pragma once

#include <vector>
#include "Point.h"
using namespace std;

class Polygon {
public:
  Polygon();

  Point centroid();

  void translate(int tx, int ty, int a = 0, int b = 0);
  void dilate(double kx, double ky, int a = 0, int b = 0);
  void rotate(double theta, int a = 0, int b = 0);

private:
  vector<Point> points;

  // Define Line from Point[from[i]] to Point[to[i]]
  vector<int> from;
  vector<int> to;
};