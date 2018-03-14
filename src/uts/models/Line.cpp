#include "Line.h"
#include <iostream>

using namespace std;

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

Point Line::intersection(const Line& l) {
  double m = gradient(), m2 = l.gradient();
  double c = (double)first.y - m*first.x, c2 = (double)l.first.y - m2*l.first.x;
  if (m == m2) return Point::UNDEF;
  if (m == 0 && (m2 == UNDEF || m2 == -UNDEF)) return Point(l.first.x, first.y);
  if (m2 == 0 && (m == UNDEF || m == -UNDEF)) return Point(first.x, l.first.y);
  double xd = (c - c2)/(m2 - m), yd = m*xd + c;
  int xi = (int)xd, yi = (int)yd;
  return Point(xi, yi);
}

bool Line::contains(const Point& p) {
  Point left = first, right = second, temp;
  if (left.x > right.x) {
    temp = left;
    left = right;
    right = temp;
  }
  if (p.x < left.x || p.x > right.x) return false;
  if (left.y > right.y) {
    temp = left;
    left = right;
    right = temp;
  }
  if (p.y < left.y || p.y > right.y) return false;
  return true;
}