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


//----- GLOBAL VARIABLES -----//
struct termios origTermios;


//----- FUNCTION DECLARATIONS -----//
Drawable readFromFile(const string& filename);
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
  buff.addShape("item", readFromFile("chars/6/MenuItem.txt"));
  buff.addShape("checkbox", readFromFile("chars/6/Checkbox.txt"));
  buff.addShape("checked", readFromFile("chars/6/Checked.txt"));
  buff.addShape("small-box", readFromFile("chars/6/Small.txt"));
  buff.addShape("big-box", readFromFile("chars/6/Big.txt"));
  buff.addShape("gedung1", readFromFile("chars/6/DummyGedung1.txt"));

  // Position config
  int xMenu1 = 50,
    yMenu1 = 50;
  int distMenu1Menu2 = 25;
  int distMenu2Big = 25;
  int xMenu2 = xMenu1 + buff.getShape("menu1").points[2].x + distMenu1Menu2,
    yMenu2 = yMenu1;
  int xItem = xMenu1 + 5,
    yItem = yMenu1 + 5,
    diffItem = buff.getShape("item").points[2].y + 4;
  int xCheckbox = xItem + 10,
    yCheckbox = yItem + 12;
  int xSmall = xMenu2 + buff.getShape("menu2").points[2].x/2,
    ySmall = yMenu2 + buff.getShape("menu2").points[2].y/2;
  int xBig = xMenu2 + buff.getShape("menu2").points[2].x + distMenu2Big,
    yBig = yMenu1;
  int move = 20,
    xminMove = xMenu2 + buff.getShape("menu2").points[0].x,
    xmaxMove = xMenu2 + buff.getShape("menu2").points[2].x - buff.getShape("small-box").points[2].x,
    yminMove = yMenu2 + buff.getShape("menu2").points[0].y,
    ymaxMove = yMenu2 + buff.getShape("menu2").points[2].y - buff.getShape("small-box").points[2].y;

  // Select menu config
  int selectedMenu = 1;
  int selectedItem = 0; // 0..nItem-1
  int nItem = 4; // total layer i.e tree, building, street, water  -->  4 layers
  bool checkbox[nItem];
  for (int i=0; i<nItem; i++) {
    checkbox[i] = true;
  }

  // Scale config
  int scale = 1,
    minScale = 1,
    maxScale = 8;

  char input = '0';
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
            
            xminMove = xMenu2 + buff.getShape("menu2").points[0].x,
            xmaxMove = xMenu2 + buff.getShape("menu2").points[2].x - buff.getShape("small-box").points[2].x*scale,
            yminMove = yMenu2 + buff.getShape("menu2").points[0].y*scale,
            ymaxMove = yMenu2 + buff.getShape("menu2").points[2].y - buff.getShape("small-box").points[2].y*scale;

            if (xSmall >= xmaxMove) {
              xSmall = xmaxMove;
            }
            if (ySmall >= ymaxMove) {
              ySmall = ymaxMove;
            }
          }
          break;
        case '.': // enlarge
          if (selectedMenu == 2) {
            scale++;
            if (scale > maxScale)
              scale = maxScale;
            
            xminMove = xMenu2 + buff.getShape("menu2").points[0].x,
            xmaxMove = xMenu2 + buff.getShape("menu2").points[2].x - buff.getShape("small-box").points[2].x*scale,
            yminMove = yMenu2 + buff.getShape("menu2").points[0].y*scale,
            ymaxMove = yMenu2 + buff.getShape("menu2").points[2].y - buff.getShape("small-box").points[2].y*scale;

            if (xSmall >= xmaxMove) {
              xSmall = xmaxMove;
            }
            if (ySmall >= ymaxMove) {
              ySmall = ymaxMove;
            }
          }
          break;
        default:
          break;
      }
    }    
    
    buff.drawShape("menu1", xMenu1, yMenu1, Color(100,100,100));
    buff.drawShape("menu2", xMenu2, yMenu2, Color(100,100,100));
    buff.drawShape("gedung1", xMenu2, yMenu2, Color::RED);
    if (selectedMenu == 1)
      buff.drawShapeBorder("menu1", xMenu1, yMenu1, Color::WHITE);
    else
      buff.drawShapeBorder("menu2", xMenu2, yMenu2, Color::WHITE);

    for (int i=0; i<nItem; i++) {
      buff.drawShape("item", xItem, yItem+diffItem*i, Color(50,50,50));
      buff.drawShapeBorder("checkbox", xCheckbox, yCheckbox+diffItem*i, Color::WHITE);
      if (checkbox[i]) {
        buff.drawShapeBorder("checked", 5+xCheckbox, 5+yCheckbox+diffItem*i, Color::WHITE);
      }
    }
    buff.drawShapeBorder("item", xItem, yItem+diffItem*selectedItem, Color::WHITE);

    buff.drawScaleShapeBorder("small-box", xSmall, ySmall, Color::WHITE, scale);
    buff.drawShapeBorder("big-box", xBig, yBig, Color::WHITE);

    buff.drawClippedShape("gedung1", xMenu2, yMenu2, "small-box", xSmall, ySmall, scale, Color::RED);

    buff.apply();
  } while (input != 'q');

  return 0;
}
