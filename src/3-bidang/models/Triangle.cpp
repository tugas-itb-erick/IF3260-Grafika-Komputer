#include "Triangle.h"
#include <stdlib.h>

Triangle::Triangle() {
  first = Point();
  second = Point();
  third = Point();
  c = Color();
}

Triangle::Triangle(Point p1, Point p2, Point p3) {
  first = p1;
  second = p2;
  third = p3;
  c = Color();
}

Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
  first = Point(x1, y1);
  second = Point(x2, y2);
  third = Point(x3, y3);
  c = Color();
}

Triangle::Triangle(Point p1, Point p2, Point p3, Color cl) {
  p1.c = p2.c = p3.c = cl;
  first = p1;
  second = p2;
  third = p3;
  c = cl;
}

Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color cl) {
  first = Point(x1, y1, cl);
  second = Point(x2, y2, cl);
  third = Point(x3, y3, cl);
  c = cl;
}

Triangle::Triangle(const Triangle& T) {
  first = T.first;
  second = T.second;
  third = T.third;
  c = T.c;
}

Triangle& Triangle::operator=(const Triangle& T) {
  first = T.first;
  second = T.second;
  third = T.third;
  c = T.c;
  return *this;
}

Point Triangle::centroid() {
  return Point((first.x + second.x + third.x)/3, (first.y + second.y + third.y)/3);
}

double Triangle::area() {
  double temp = (double) (first.x*(second.y-third.y) + second.x*(third.y-first.y)+ third.x*(first.y-second.y))/2.0;
  if (temp < 0) temp = -temp;
  return temp;
}

bool Triangle::hasPoint(Point P) {
  double temp = area() - Triangle(P, second, third).area() - Triangle(P, first, third).area() - Triangle(P, first, second).area();
  if (temp < 0) temp = -temp;
  return temp < 0.0001;
}
