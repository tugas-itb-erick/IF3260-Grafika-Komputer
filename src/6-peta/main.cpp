/*
13515021 - Dewita Sonya Tarabunga
13515057 - Erick Wijaya
13515063 - Kezia Suhendra
13515078 - Veren Iliana
13515087 - Audry Nyonata
13515111 - Catherine Almira
13515144 - William
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <termios.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <fstream>
#include "models/Color.h"
#include "models/Buffer.h"
using namespace std;


//----- CONSTANTS -----//
#define PI 3.14159265
#define DOUBLE2INT_CORRECTION_VAL 0.5
#define UNDEFINED -1
#define SCALECONSTANT 1.2
#define LOOP 2


//----- GLOBAL VARIABLES -----//
struct termios origTermios;


//----- FUNCTION DECLARATIONS -----//
Drawable readFromFile(const string& filename);
int parabolaX(double initSpeed, double theta, double time);
int parabolaY(double initSpeed, double theta, double time, double accel);
int freefall(double initSpeed, double time, double accel);
void resetTerminalMode();
void setConioTerminalNode();
int kbhit();


//----- FUNCTION IMPLEMENTATIONS -----//
void resetTerminalMode() {
  tcsetattr(0, TCSANOW, &origTermios);
}

void setConioTerminalNode() {
  struct termios newTermios;

  /* take two copies - one for now, one for later */
  tcgetattr(0, &origTermios);
  memcpy(&newTermios, &origTermios, sizeof(newTermios));

  /* register cleanup handler, and set the new terminal mode */
  atexit(resetTerminalMode);
  cfmakeraw(&newTermios);
  tcsetattr(0, TCSANOW, &newTermios);
}

int kbhit() {
  struct timeval tv = { 0L, 0L };
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(0, &fds);
  return select(1, &fds, NULL, NULL, &tv);
}

Drawable readFromFile(const string& filename) {
  ifstream infile(filename);
  if (infile.fail()) {
    // file not found, access denied, etc
  } else {
    int nPoint;
    vector<Point> vp;

    // Read points
    infile >> nPoint;
    for (int i=0; i<nPoint + 1; i++) {
      int x, y;
      infile >> x >> y;
      vp.push_back(Point(x, y));
    }

    infile.close();
    return Drawable(vp);
  }
  return Drawable();
}


//----- MAIN PROGRAM -----//
int main() {
	
  Buffer buff;
  buff.reset();
  setConioTerminalNode();
  
  Drawable view1 = readFromFile("chars/6/Kotak.txt");
  Drawable view2 = readFromFile("chars/6/PersegiPanjang.txt");
  buff.addShape("menu1", view1);
  buff.addShape("k1", view1);buff.addShape("k2", view1);
  buff.addShape("menu2", view2);

  char menu = '0';
  char input = '0';

  do {
    buff.reset();
    if (kbhit()) {
      read(0, &input, sizeof(input));
      menu = input;
    }
    switch (menu) {
      case '1':
        buff.drawShape("menu1", 100, 100, Color(100,100,100));
        buff.drawShape("menu2", 100, 300, Color(50,50,50));
        break;
      case '2':
        buff.drawShape("menu1", 100, 100, Color(50,50,50));
        buff.drawShape("menu2", 100, 300, Color(100,100,100));
        break;
      default:
        buff.drawShape("menu1", 100, 100, Color(50,50,50));
        buff.drawShape("menu2", 100, 300, Color(50,50,50));
        break;
    }

    buff.drawShape("k1", 500, 100, Color::RED);
    buff.drawShape("k2", 600, 100, Color::BLUE);

    buff.drawScaleShape("k1", 500, 400, Color::RED, 2, 600, 450);
    buff.drawScaleShape("k2", 600, 400, Color::BLUE, 2, 600, 450);

    buff.apply();
  } while (input != 'q');

  return 0;
}
