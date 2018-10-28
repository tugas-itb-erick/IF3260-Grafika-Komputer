#pragma once

#include <vector>
#include "Point.h"
#include "Triangle.h"
using namespace std;

class CharDrawable {
public:
  char letter;
  vector<Point> points;
  vector<Triangle> triangles;

  CharDrawable();
  CharDrawable(char, Point*, Triangle*);
  CharDrawable(char, vector<Point>, vector<Triangle>);
  CharDrawable(const CharDrawable&);
  CharDrawable& operator=(const CharDrawable&);

  void useColor(Color);
};