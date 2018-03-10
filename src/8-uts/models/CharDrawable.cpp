#include "CharDrawable.h"

CharDrawable::CharDrawable() {
  letter = '#';
  nLines = 0;
  lines = nullptr;
}

CharDrawable::CharDrawable(char letter, int nLines, Line* lines) {
  this->letter = letter;
  this->nLines = nLines;
  this->lines = new Line[nLines];
  for (int i=0; i<nLines; i++) {
    this->lines[i] = lines[i];
  }
}

CharDrawable::CharDrawable(const CharDrawable& ch) {
  letter = ch.letter;
  nLines = ch.nLines;
  lines = new Line[nLines];
  for (int i=0; i<nLines; i++) {
    lines[i] = ch.lines[i];
  }
}

CharDrawable& CharDrawable::operator=(const CharDrawable& ch) {
  if (this == &ch)
    return *this;
  letter = ch.letter;
  nLines = ch.nLines;
  lines = new Line[nLines];
  for (int i=0; i<nLines; i++) {
    lines[i] = ch.lines[i];
  }
  return *this;
}

void CharDrawable::useColor(const Color& c) {
  for (int i=0; i<nLines; i++) {
    lines[i].c = c;
  }
}