#include "Drawable.h"
using namespace std;

Drawable::Drawable() {
  points = vector<Point>();
}

Drawable::Drawable(Point* points) {
  this->points = vector<Point>(points, points + sizeof(points)/sizeof(points[0]));
}

Drawable::Drawable(vector<Point> points) {
  this->points = points;
}

Drawable::Drawable(const Drawable& ch) {
  points = ch.points;
}

Drawable& Drawable::operator=(const Drawable& ch) {
  points = ch.points;
  return *this;
}

void Drawable::useColor(Color c) {
  for (int i=0; i<points.size(); i++)
    points[i].c = c;
}

Point Drawable::centroid() {
  return points.back();
}