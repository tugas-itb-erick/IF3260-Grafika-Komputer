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
  Point cen = Point(0, 0);
  for (int i=0;i<points.size()-1;++i) {
  	cen += points[i];
  }
  cen.x /= points.size()-1;
  cen.y /= points.size()-1;
  return cen;
}