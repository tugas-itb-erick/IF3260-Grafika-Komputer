#include "Color.h"

const Color Color::WHITE = Color();
const Color Color::BLACK = Color(0, 0, 0, 0);
const Color Color::RED = Color(255, 0, 0, 0);
const Color Color::GREEN = Color(0, 255, 0, 0);
const Color Color::BLUE = Color(0, 0, 255, 0);
// dst

Color::Color() {
  r = g = b = 255;
  a = 0;
}

Color::Color(int r, int g, int b, int a) {
  this->r = r;
  this->g = g;
  this->b = b;
  this->a = a;
}

Color::Color(const Color& c) {
  r = c.r;
  g = c.g;
  b = c.b;
  a = c.a;
}

Color& Color::operator=(const Color& c) {
  r = c.r;
  g = c.g;
  b = c.b;
  a = c.a;
  return *this;
}

