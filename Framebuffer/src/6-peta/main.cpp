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
#include <fcntl.h>
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

Drawable readShapeFromFile(const string& filename) {
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

vector<Drawable> readLayerFromFile(const string& filename) {
  vector<Drawable> vd;
  ifstream infile(filename);
  if (!infile.fail()) {
    int nPoint, x ,y;
    vector<Point> vp;
    while (1) {
      infile >> nPoint;
      if (nPoint==0) break;
      ++nPoint;
      vp.clear();
      while (nPoint--) {
        infile >> x >> y;
        vp.push_back(Point(x, y));
      }
      vd.push_back(Drawable(vp));
    }
    infile.close();
  }
  return vd;
}

//----- MAIN PROGRAM -----//
int main() {
  Buffer buff;
  buff.reset();
  setConioTerminalNode();
  
  // Setup shapes
  buff.addShape("menu1", readShapeFromFile("chars/6/Menu1.txt"));
  buff.addShape("menu2", readShapeFromFile("chars/6/Menu2.txt"));
  buff.addShape("item", readShapeFromFile("chars/6/MenuItem.txt"));
  buff.addShape("checkbox", readShapeFromFile("chars/6/Checkbox.txt"));
  buff.addShape("checked", readShapeFromFile("chars/6/Checked.txt"));
  buff.addShape("small-box", readShapeFromFile("chars/6/Small.txt"));
  buff.addShape("big-box", readShapeFromFile("chars/6/Big.txt"));
  buff.addShape("jalan", readShapeFromFile("chars/6/jalan.txt"));

  // Setup layers
  buff.addLayer("gedung", readLayerFromFile("chars/6/gedung.txt"));
  buff.addLayer("jalan", readLayerFromFile("chars/6/jalan.txt"));
  buff.addLayer("lapangan", readLayerFromFile("chars/6/lapangan.txt"));
  
  // Scale config
  int scale = 2,
    minScale = 1,
    maxScale = 5;
  
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
    xmaxMove = xMenu2 + buff.getShape("menu2").points[2].x - buff.getShape("small-box").points[2].x*scale,
    yminMove = yMenu2 + buff.getShape("menu2").points[0].y,
    ymaxMove = yMenu2 + buff.getShape("menu2").points[2].y - buff.getShape("small-box").points[2].y*scale;

  // Select menu config
  int selectedMenu = 2;
  int selectedItem = 0; // 0..nItem-1
  int nItem = 4; // total layer 0: building, 1: street, 2: tree  3: lapangan-->  4 layers
  Color treeColor = Color(0,55,0);
  Color streetColor = Color::GRAY;
  Color buildingColor = Color::ORANGE;
  Color lapanganColor = Color::GREEN;
  

  // Setup layer checkboxes
  bool checkbox[nItem];
  for (int i=0; i<nItem; i++) {
    checkbox[i] = true;
  }
  const int TREE = 2, JALAN = 1, GEDUNG = 0, LAPANGAN = 3;
  char input = '0';

  do {
    buff.reset();

    // Checkboxes
  	if (checkbox[TREE]) {
  		buff.drawShape("menu2", xMenu2, yMenu2, treeColor);
  		buff.drawClippedShape("menu2", xMenu2, yMenu2, "small-box", xSmall, ySmall, scale, xBig, yBig, treeColor);  
  	}
  	if (checkbox[JALAN]) {
  		buff.drawLayer("jalan", xMenu2+20, yMenu2+100, streetColor);
  		buff.drawClippedLayer("jalan", xMenu2+20, yMenu2+100, "small-box", xSmall, ySmall, scale, xBig, yBig, streetColor);
  	}	
  	if (checkbox[GEDUNG]) {
  		buff.drawLayer("gedung", xMenu2+20, yMenu2+100, buildingColor);
  		buff.drawClippedLayer("gedung", xMenu2+20, yMenu2+100, "small-box", xSmall, ySmall, scale, xBig, yBig, buildingColor);
  	}    
  	if (checkbox[LAPANGAN]) {
  		buff.drawLayer("lapangan", xMenu2+20, yMenu2+100, lapanganColor);
  		buff.drawClippedLayer("lapangan", xMenu2+20, yMenu2+100, "small-box", xSmall, ySmall, scale, xBig, yBig, lapanganColor);
  	}

    // Draw menu 1, layer checkboxes
    buff.drawShape("menu1", xMenu1, yMenu1, Color(50,50,50));
    buff.drawShape("item", xItem, yItem+diffItem*0, buildingColor); //checkbox building
    buff.drawShape("item", xItem, yItem+diffItem*1, streetColor); //checkbox street
    buff.drawShape("item", xItem, yItem+diffItem*2, treeColor); //checkbox tree
    buff.drawShape("item", xItem, yItem+diffItem*3, lapanganColor); //checkbox tree
    for (int i=0; i<nItem; i++) {
      buff.drawShapeBorder("checkbox", xCheckbox, yCheckbox+diffItem*i, Color::WHITE);
      if (checkbox[i]) {
        buff.drawShapeBorder("checked", 5+xCheckbox, 5+yCheckbox+diffItem*i, Color::WHITE);
      }
    }

	  // DIGAMBAR DIATAS SEGALA GALANYA
    if (selectedMenu == 1) {
  		buff.drawShapeBorder("item", xItem, yItem+diffItem*selectedItem, Color::YELLOW);
  		buff.drawShapeBorder("checkbox", xCheckbox, yCheckbox+diffItem*selectedItem, Color::YELLOW);
  		if (checkbox[selectedItem]) {
  			buff.drawShapeBorder("checked", 5+xCheckbox, 5+yCheckbox+diffItem*selectedItem, Color::YELLOW);
  		}
  		buff.drawScaleShapeBorder("small-box", xSmall, ySmall, Color::WHITE, scale);
      
	  } else  {
  		buff.drawShapeBorder("item", xItem, yItem+diffItem*selectedItem, Color::WHITE);
  		buff.drawScaleShapeBorder("small-box", xSmall, ySmall, Color::YELLOW, scale);
  	}
	  buff.drawShapeBorder("menu1", xMenu1, yMenu1, Color::WHITE);
    buff.drawShapeBorder("menu2", xMenu2, yMenu2, Color::WHITE);    
    buff.drawShapeBorder("big-box", xBig, yBig, Color::WHITE);
	

    buff.apply();

    // Keyboard input
    while (!kbhit()) {};
      read(0, &input, sizeof(input));
      switch (input) {
        case '1': // Switch to menu layer checkboxes
          selectedMenu = 1;
          break;
        case '2': // Switch to minimap
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
  } while (input != 'q');
  return 0;
}
