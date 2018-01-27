#include "Line.h"

const double Line::UNDEF = 99999.99;

Line::Line() {
  start = Point();
  end = Point();
  c = Color();
}

Line::Line(Point p1, Point p2) {
  start = p1;
  end = p2;
  c = Color();
}

Line::Line(int x1, int y1, int x2, int y2) {
  start = Point(x1, y1);
  end = Point(x2, y2);
  c = Color();
}

Line::Line(Point p1, Point p2, const Color& cl) {
  p1.c = p2.c = cl;
  start = p1;
  end = p2;
  c = cl;
}

Line::Line(int x1, int y1, int x2, int y2, const Color& cl) {
  start = Point(x1, y1, cl);
  end = Point(x2, y2, cl);
  c = cl;
}

Line::Line(const Line& L) {
  start = L.start;
  end = L.end;
  c = L.c;
}

Line& Line::operator=(const Line& L) {
  start = L.start;
  end = L.end;
  c = L.c;
  return *this;
}

double Line::gradient() {
  if (start.x - end.x == 0)
    return UNDEF;
  return (double) (start.y - end.y) / (start.x - end.x);
}
