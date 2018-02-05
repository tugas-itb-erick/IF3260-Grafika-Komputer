#include "Drawable.h"
using namespace std;

Drawable::Drawable() {
  points = vector<Point>();
  triangles = vector<Triangle>();
}

Drawable::Drawable(Point* points, Triangle* triangles) {
  this->points = vector<Point>(points, points + sizeof(points)/sizeof(points[0]));
  this->triangles = vector<Triangle>(triangles, triangles + sizeof(triangles)/sizeof(triangles[0]));
}

Drawable::Drawable(vector<Point> points, vector<Triangle> triangles) {
  this->points = points;
  this->triangles = triangles;
}

Drawable::Drawable(const Drawable& ch) {
  points = ch.points;
  triangles = ch.triangles;
}

Drawable& Drawable::operator=(const Drawable& ch) {
  points = ch.points;
  triangles = ch.triangles;
  return *this;
}

void Drawable::useColor(Color c) {
  for (int i=0; i<points.size(); i++)
    points[i].c = c;
  for (int i=0; i<triangles.size(); i++)
    triangles[i].c = c;
}

Point Drawable::centroid() {
  return points.back();
}