#include "Color.h"

const Color Color::WHITE  = Color(255, 255, 255);
const Color Color::BLACK  = Color(0, 0, 0);
const Color Color::RED    = Color(255, 0, 0);
const Color Color::ORANGE = Color(255, 128, 0);
const Color Color::YELLOW = Color(255, 255, 0);
const Color Color::GREEN  = Color(0, 255, 0);
const Color Color::BLUE   = Color(0, 0, 255);
const Color Color::PINK   = Color(255, 105, 180);
const Color Color::PURPLE = Color(128, 0, 256);
const Color Color::GRAY = Color(100,100,100);
const Color Color::NAVY = Color(0,0,80);

Color::Color() {
  r = g = b = 0;
  a = 0;
}

Color::Color(int r, int g, int b) {
  this->r = r;
  this->g = g;
  this->b = b;
  this->a = 0;
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

bool Color::operator==(const Color& c) {
  return (r == c.r && g == c.g && b == c.b && a == c.a);
}

