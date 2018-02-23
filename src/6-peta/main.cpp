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
  
  buff.addShape("menu1", readFromFile("chars/6/Menu1.txt"));
  buff.addShape("menu2", readFromFile("chars/6/Menu2.txt"));
  buff.addShape("item-tree", readFromFile("chars/6/MenuItem.txt"));
  buff.addShape("item-street", readFromFile("chars/6/MenuItem.txt"));
  buff.addShape("item-water", readFromFile("chars/6/MenuItem.txt"));
  buff.addShape("item-building", readFromFile("chars/6/MenuItem.txt"));
  buff.addShape("small-box", readFromFile("chars/6/Small.txt"));
  buff.addShape("map-box", readFromFile("chars/6/Big.txt"));

  char menu = '0';
  char input = '0';
  int xMenu1 = 50,
    yMenu1 = 50;
  int xMenu2 = xMenu1,
    yMenu2 = yMenu1 + 240;
  int xItem = xMenu1 + 5,
    yItem = yMenu1 + 5,
    diffItem = 54;
  int xSmall = 200,
    ySmall = 150 + yMenu1 + diffItem*3;
  int xMap = 500,
    yMap = yMenu1;
  int move = 20,
    xminMove = xMenu2 + buff.getShape("menu2").points[0].x,
    xmaxMove = xMenu2 + buff.getShape("menu2").points[2].x - buff.getShape("small-box").points[2].x,
    yminMove = yMenu2 + buff.getShape("menu2").points[0].y,
    ymaxMove = yMenu2 + buff.getShape("menu2").points[2].y - buff.getShape("small-box").points[2].y;

  int selectedMenu = 1;
  int selectedItem = 0; // 0..nItem-1
  int nItem = 4;
  bool checkbox[nItem];
  for (auto el : checkbox) {
    el = true;
  }

  int scale = 1,
    minScale = 1,
    maxScale = 4;

  do {
    buff.reset();
    if (kbhit()) {
      read(0, &input, sizeof(input));
      switch (input) {
        case '1':
          selectedMenu = 1;
          break;
        case '2':
          selectedMenu = 2;
          break;
        case 'a': // left
          if (selectedMenu == 2) {
            xSmall -= move;
            if (xSmall < xminMove)
              xSmall = xminMove;
          }
          break;
        case 'd': // right
          if (selectedMenu == 2) {
            xSmall += move;
            if (xSmall > xmaxMove)
              xSmall = xmaxMove;
          }
          break;
        case 'w': // up
          if (selectedMenu == 1) {
            --selectedItem;
            if (selectedItem < 0)
              selectedItem += nItem;
          } else {
            ySmall -= move;
            if (ySmall < yminMove)
              ySmall = yminMove;
          }
          break;
        case 's': // down
          if (selectedMenu == 1) {
            ++selectedItem;
            selectedItem %= nItem;
          } else {
            ySmall += move;
            if (ySmall > ymaxMove)
              ySmall = ymaxMove;
          }
          break;
        case ' ': // check/uncheck layer
          if (selectedMenu == 1) {
            checkbox[selectedItem] = !checkbox[selectedItem];
          }
          break;
        case ',': // shrink
          if (selectedMenu == 2) {
            scale--;
            if (scale < minScale)
              scale = minScale;
            // TODO: handle min max value and reposition "small-box" if out of bound
          }
          break;
        case '.': // enlarge
          if (selectedMenu == 2) {
            scale++;
            if (scale > maxScale)
              scale = maxScale;
            // TODO: handle min max value and reposition "small-box" if out of bound
          }
          break;
        default:
          break;
      }
    }
    
    buff.drawShape("menu1", xMenu1, yMenu1, Color(100,100,100));
    buff.drawShape("menu2", xMenu2, yMenu2, Color(100,100,100));
    if (selectedMenu == 1)
      buff.drawShapeBorder("menu1", xMenu1, yMenu1, Color::WHITE);
    else
      buff.drawShapeBorder("menu2", xMenu2, yMenu2, Color::WHITE);

    buff.drawShape("item-tree", xItem, yItem, Color(50,50,50));
    buff.drawShape("item-street", xItem, yItem+diffItem, Color(50,50,50));
    buff.drawShape("item-water", xItem, yItem+diffItem*2, Color(50,50,50));
    buff.drawShape("item-building", xItem, yItem+diffItem*3, Color(50,50,50));
    switch (selectedItem) {
      case 0: buff.drawShapeBorder("item-tree", xItem, yItem+diffItem*selectedItem, Color::WHITE); break;
      case 1: buff.drawShapeBorder("item-street", xItem, yItem+diffItem*selectedItem, Color::WHITE); break;
      case 2: buff.drawShapeBorder("item-water", xItem, yItem+diffItem*selectedItem, Color::WHITE); break;
      case 3: buff.drawShapeBorder("item-building", xItem, yItem+diffItem*selectedItem, Color::WHITE); break;
      // case nItem-1: ...
      default: break;
    }

    buff.drawScaleShapeBorder("small-box", xSmall, ySmall, Color::WHITE, scale);
    buff.drawShapeBorder("map-box", xMap, yMap, Color::WHITE);

    buff.apply();
  } while (input != 'q');

  return 0;
}
