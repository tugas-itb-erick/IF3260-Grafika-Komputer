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
#include <fcntl.h>
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
void delay(int number_of_seconds);


void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
 
    // Stroing start time
    clock_t start_time = clock();
 
    // looping till required time is not acheived
    while (clock() < start_time + milli_seconds) {
        
	}
}

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

bool isEmpty(unsigned char *c, int s) {
  for (int i=0;i<s;++i) {
    if (c[i]) {
      return false;
    }
  }
  return true;
}

//----- MAIN PROGRAM -----//
int main() {
  Buffer buff;
  buff.reset();

  buff.addShape("sys1", readShapeFromFile("chars/7/monika"));
  buff.addShape("sys2", readShapeFromFile("chars/7/monika"));
  buff.addShape("sys3", readShapeFromFile("chars/7/monika"));
  buff.addShape("sys4", readShapeFromFile("chars/7/monika"));
  buff.addShape("sys5", readShapeFromFile("chars/7/monika"));
  buff.addShape("sys6", readShapeFromFile("chars/7/monika"));
  buff.addShape("sys7", readShapeFromFile("chars/7/monika"));
  buff.addShape("border", readShapeFromFile("chars/7/justmonika"));
  buff.addShape("mouse", readShapeFromFile("chars/7/crosshair.txt"));
  buff.addShape("mouse-click", readShapeFromFile("chars/7/crosshair-click.txt"));

  // Mouse setup
  const char *pDevice = "/dev/input/mice";
  int fmouse = open(pDevice, O_RDONLY | O_NONBLOCK);
  if (fmouse == -1) {
    printf("Error opening %s\n", pDevice);
    return -1;
  }
  int mousex = 700, mousey = 350;

  char input = '0';
  unsigned char event[3];
  signed char relx, rely;
  int clickLeft = false, clickRight = false;
  
  do {
    buff.reset();
    buff.drawShape("mouse", mousex, mousey, Color::WHITE);
    if (clickLeft) {
      buff.drawShape("mouse-click", mousex, mousey, Color::RED);
    }

    int startX = 100, startY = 300;
    int delta = 175;
    int sys1X = startX+delta*0, sys1Y = startY;
    int sys2X = startX+delta*1, sys2Y = startY;
    int sys3X = startX+delta*2, sys3Y = startY;
    int sys4X = startX+delta*3, sys4Y = startY;
    int sys5X = startX+delta*4, sys5Y = startY;
    int sys6X = startX+delta*5, sys6Y = startY;
    int sys7X = startX+delta*6, sys7Y = startY;
    buff.drawShape("sys1", sys1X, sys1Y, Color::RED);
    buff.drawShape("sys2", sys2X, sys2Y, Color::ORANGE);
    buff.drawShape("sys3", sys3X, sys3Y, Color::YELLOW);
    buff.drawShape("sys4", sys4X, sys4Y, Color::GREEN);
    buff.drawShape("sys5", sys5X, sys5Y, Color::BLUE);
    buff.drawShape("sys6", sys6X, sys6Y, Color::PINK);
    buff.drawShape("sys7", sys7X, sys7Y, Color::PURPLE);
    buff.drawShapeBorder("monika", 50, 50, Color::WHITE);

    bool detectInput = false;
    for (int i=0;i<sizeof(event);++i) {
      event[i] = 0;
    }
    input = 0;
    
    while (!detectInput) {
      if (kbhit()) {
        detectInput = true;
        read(0, &input, sizeof(input));
      }
      if (read(fmouse, event, sizeof(event))) {
        detectInput = 1;
      }
    }

    // Mouse input
    if (!isEmpty(event, sizeof(event))) {
      clickLeft = event[0] & 0x1;
      clickRight = event[0] & 0x2;
      relx = event[1];
      rely = event[2];
	
      // Position
      mousex += relx;
      mousey -= rely;
      mousex %= buff.getWidth();
      if (mousex < 0) mousex += buff.getWidth();
      mousey %= buff.getHeight();
      if (mousey < 0) mousey += buff.getHeight();
	
      if (clickLeft) {
        if (mousex > sys1X && mousex < sys1X+delta && mousey > sys1Y && mousey < sys1Y+delta) {
          system("./bin/1");
          clickRight = true;
        }
        if (mousex > sys2X && mousex < sys2X+delta && mousey > sys2Y && mousey < sys2Y+delta) {
          system("./bin/2");
          clickRight = true;
        }
        if (mousex > sys3X && mousex < sys3X+delta && mousey > sys3Y && mousey < sys3Y+delta) {
          system("./bin/3");
          clickRight = true;
        }
        if (mousex > sys4X && mousex < sys4X+delta && mousey > sys4Y && mousey < sys4Y+delta) {
          system("./bin/4");
          clickRight = true;
        }
        if (mousex > sys5X && mousex < sys5X+delta && mousey > sys5Y && mousey < sys5Y+delta) {
          system("./bin/5");
          clickRight = true;
        }
        if (mousex > sys6X && mousex < sys6X+delta && mousey > sys6Y && mousey < sys6Y+delta) {
          system("./bin/6");
          clickRight = true;
        }
        if (mousex > sys7X && mousex < sys7X+delta && mousey > sys7Y && mousey < sys7Y+delta) {
          system("./bin/7");
          clickRight = true;
        }
      }
    }
    buff.apply();
  } while (!clickRight);

  return 0;
}
