#pragma once

class Color {
public:
  int r;
  int g;
  int b;
  int a;

  Color();
  Color(int, int, int);
  Color(int, int, int, int);
  Color(const Color&);
  Color& operator=(const Color&);
  bool operator==(const Color&);

  static const Color WHITE;
  static const Color BLACK;
  static const Color RED;
  static const Color ORANGE;
  static const Color YELLOW;
  static const Color GREEN;
  static const Color BLUE;
  static const Color PINK;
  static const Color PURPLE;
  static const Color GRAY;
};