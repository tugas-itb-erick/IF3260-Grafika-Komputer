#include "Point.h"

Point::Point() {
  x = 0;
  y = 0;
}

Point::Point(int x, int y) {
  this->x = x;
  this->y = y;
}

Point::Point(const Point& p) {
  x = p.x;
  y = p.y;
}

Point& Point::operator=(const Point& p) {
  x = p.x;
  y = p.y;
  return *this;
}

Point Point::operator+(const Point& p) {
  return Point(x + p.x, y + p.y);
}

void Point::translate(int tx, int ty, int a, int b) {

}

void Point::dilate(double kx, double ky, int a, int b) {

}

void Point::rotate(double theta, int a, int b) {

}
