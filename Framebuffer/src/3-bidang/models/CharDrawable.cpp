#include "CharDrawable.h"
using namespace std;

CharDrawable::CharDrawable() {
  letter = '?';
  points = vector<Point>();
  triangles = vector<Triangle>();
}

CharDrawable::CharDrawable(char letter, Point* points, Triangle* triangles) {
  this->letter = letter;
  this->points = vector<Point>(points, points + sizeof(points)/sizeof(points[0]));
  this->triangles = vector<Triangle>(triangles, triangles + sizeof(triangles)/sizeof(triangles[0]));
}

CharDrawable::CharDrawable(char letter, vector<Point> points, vector<Triangle> triangles) {
  this->letter = letter;
  this->points = points;
  this->triangles = triangles;
}

CharDrawable::CharDrawable(const CharDrawable& ch) {
  letter = ch.letter;
  points = ch.points;
  triangles = ch.triangles;
}

CharDrawable& CharDrawable::operator=(const CharDrawable& ch) {
  letter = ch.letter;
  points = ch.points;
  triangles = ch.triangles;
  return *this;
}

void CharDrawable::useColor(Color c) {
  for (int i=0; i<points.size(); i++)
    points[i].c = c;
  for (int i=0; i<triangles.size(); i++)
    triangles[i].c = c;
}