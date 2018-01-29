#pragma once

#include <vector>
#include "Line.h"
#include "Triangle.h"
using namespace std;

class CharDrawable {
public:
  char letter;
  vector<Line> lines;
  vector<Triangle> triangles;

  CharDrawable();
  CharDrawable(char, Line*, Triangle*);
  CharDrawable(char, vector<Line>, vector<Triangle>);
  CharDrawable(const CharDrawable&);
  CharDrawable& operator=(const CharDrawable&);

  void useColor(const Color&);
};