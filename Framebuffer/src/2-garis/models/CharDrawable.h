#pragma once

#include "Line.h"

class CharDrawable {
public:
  char letter;
  int nLines;
  Line* lines;

  CharDrawable();
  CharDrawable(char, int, Line*);
  CharDrawable(const CharDrawable&);
  CharDrawable& operator=(const CharDrawable&);

  void useColor(const Color&);
};