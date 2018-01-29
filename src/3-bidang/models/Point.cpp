#include "Point.h"

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

Point& Point::operator=(const Point& p) {
  x = p.x;
  y = p.y;
  c = p.c;
  return *this;
}
