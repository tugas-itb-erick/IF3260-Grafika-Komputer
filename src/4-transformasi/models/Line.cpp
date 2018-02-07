#include "Line.h"

const double Line::UNDEF = 100000;

Line::Line() {
  first = Point();
  second = Point();
  c = Color();
}

Line::Line(Point p1, Point p2) {
  first = p1;
  second = p2;
  c = Color();
}

Line::Line(int x1, int y1, int x2, int y2) {
  first = Point(x1, y1);
  second = Point(x2, y2);
  c = Color();
}

Line::Line(Point p1, Point p2, Color cl) {
  p1.c = p2.c = cl;
  first = p1;
  second = p2;
  c = cl;
}

Line::Line(int x1, int y1, int x2, int y2, Color cl) {
  first = Point(x1, y1, cl);
  second = Point(x2, y2, cl);
  c = cl;
}

Line::Line(const Line& L) {
  first = L.first;
  second = L.second;
  c = L.c;
}

Line& Line::operator=(const Line& L) {
  first = L.first;
  second = L.second;
  c = L.c;
  return *this;
}

double Line::gradient() const {
  if (first.x - second.x == 0) {
    if (first.y >= second.y)
      return UNDEF;
    else
      return -UNDEF;
  }
  return (double) (first.y - second.y) / (first.x - second.x);
}
