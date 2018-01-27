#pragma once

class Color {
public:
  int r;
  int g;
  int b;
  int a;

  Color();
  Color(int, int, int, int);
  Color(const Color&);
  Color& operator=(const Color&);

  static const Color WHITE;
  static const Color BLACK;
  static const Color RED;
  static const Color GREEN;
  static const Color BLUE;
  // dst
};