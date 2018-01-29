#include "CharDrawable.h"
using namespace std;

CharDrawable::CharDrawable() {
  letter = '?';
  lines = vector<Line>();
  triangles = vector<Triangle>();
}

CharDrawable::CharDrawable(char letter, Line* lines, Triangle* triangles) {
  this->letter = letter;
  this->lines = vector<Line>(lines, lines + sizeof(lines)/sizeof(lines[0]));
  this->triangles = vector<Triangle>(triangles, triangles + sizeof(triangles)/sizeof(triangles[0]));
}

CharDrawable::CharDrawable(char letter, vector<Line> lines, vector<Triangle> triangles) {
  this->letter = letter;
  this->lines = lines;
  this->triangles = triangles;
}

CharDrawable::CharDrawable(const CharDrawable& ch) {
  letter = ch.letter;
  lines = ch.lines;
  triangles = ch.triangles;
}

CharDrawable& CharDrawable::operator=(const CharDrawable& ch) {
  letter = ch.letter;
  lines = ch.lines;
  triangles = ch.triangles;
  return *this;
}

void CharDrawable::useColor(const Color& c) {
  
}