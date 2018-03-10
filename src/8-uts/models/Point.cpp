#include "Point.h"

const Point Point::UNDEF = Point(-100, -100);

Point::Point() {
  x = 0;
  y = 0;
  c = Color();
}

Point::Point(int x, int y) {
  this->x = x;
  this->y = y;
  c = Color();
}

Point::Point(int x, int y, Color c) {
  this->x = x;
  this->y = y;
  this->c = c;
}

Point::Point(const Point& p) {
  x = p.x;
  y = p.y;
  c = p.c;
}

void Point::negate() {
  x = -x;
  y = -y;
}

bool Point::operator==(const Point& p) {
  return (x == p.x && y == p.y);
}

Point& Point::operator=(const Point& p) {
  x = p.x;
  y = p.y;
  c = p.c;
  return *this;
}

Point Point::operator+(const Point& p) {
  Point res;
  res.x = this->x + p.x;
  res.y = this->y + p.y;
  return res;
}

Point Point::operator-(const Point& p) {
  Point res;
  res.x = this->x - p.x;
  res.y = this->y - p.y;
  return res;
}

Point& Point::operator*=(double k) {
  double xx = x, yy = y;
  xx *= k;
  yy *= k;
  x = (int)xx;
  y = (int)yy;
  return *this;
}

Point& Point::operator+=(const Point& p) {
  x += p.x;
  y += p.y;
  return *this;
}

Point& Point::operator-=(const Point& p) {
  x -= p.x;
  y -= p.y;
  return *this;
}

bool Point::isUndefined() {
  return operator==(UNDEF);
}

bool Point::isDefined() {
  return !isUndefined();
}