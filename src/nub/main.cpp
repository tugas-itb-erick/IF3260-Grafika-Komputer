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

  buff.addShape("sys", readShapeFromFile("chars/7/monika"));
  buff.addShape("kirikanan", readShapeFromFile("chars/7/monikamm"));
  buff.addShape("atasbawah", readShapeFromFile("chars/7/monikammm"));
  buff.addShape("border", readShapeFromFile("chars/7/justmonika"));
  buff.addShape("mouse", readShapeFromFile("chars/7/crosshair.txt"));
  buff.addShape("mouse-click", readShapeFromFile("chars/7/crosshair-click.txt"));

  buff.addShape("eye", readShapeFromFile("chars/4/Alien_Eye.txt"));
  buff.addShape("cockpit" , readShapeFromFile("chars/4/Ufo_Cockpit.txt"));
  buff.addShape("face", readShapeFromFile("chars/4/Alien_Face.txt"));
  buff.addShape("neck", readShapeFromFile("chars/4/Ufo_Neck.txt"));
  buff.addShape("upperbody", readShapeFromFile("chars/4/Ufo_UpperBody.txt"));
  buff.addShape("lowerbody", readShapeFromFile("chars/4/Ufo_LowerBody.txt"));

  // Mouse setup
  const char *pDevice = "/dev/input/mice";
  int fmouse = open(pDevice, O_RDONLY | O_NONBLOCK);
  if (fmouse == -1) {
    printf("Error opening %s\n", pDevice);
    return -1;
  }
  int mousex = 700, mousey = 500;

  char input = '0';
  unsigned char event[3];
  signed char relx, rely;
  int clickLeft = 0, clickRight;
  bool stop = false;
  
  int alien1X = 100;
  int alien1Y = 100;
  int alien2X = 1200;
  int alien2Y = 100;
  int alien3X = 300;
  int alien3Y = 300;
  int alien4X = 500;
  int alien4Y = 500;
  int alien5X = 250;
  int alien5Y = 250;
  int alien6X = 600;
  int alien6Y = 600;
  
  do {
    buff.reset();

    int startX = 100, startY = 300;
    int delta = 175;
    int sys1X = startX+delta*0, sys1Y = startY;
    int sys2X = startX+delta*1, sys2Y = startY;
    int sys3X = startX+delta*2, sys3Y = startY;
    int sys4X = startX+delta*3, sys4Y = startY;
    int sys5X = startX+delta*4, sys5Y = startY;
    int sys6X = startX+delta*5, sys6Y = startY;
    int sys7X = startX+delta*6, sys7Y = startY;
    buff.drawShape("border", 50, 50, Color::BLACK);
    
    {
      for (int i=0; i<75; i++) {
        buff.drawPoint(rand()%1300+50, rand()%650+50, Color::RED);
        buff.drawPoint(rand()%1300+50, rand()%650+50, Color::WHITE);
        buff.drawPoint(rand()%1300+50, rand()%650+50, Color::YELLOW);
        buff.drawPoint(rand()%1300+50, rand()%650+50, Color::BLUE);
      }
    }

    {
      int alienXmin = 25;
      int alienYmin = 25;
      int alienXmax = 1300;
      int alienYmax = 650;

      alien1X += 20;
      alien1X = (alien1X >= alienXmin)? (alien1X <= alienXmax)? alien1X : alienXmin : alienXmax;
      buff.drawScaleShape("cockpit", alien1X, alien1Y, Color::WHITE, 1.5);
      buff.drawScaleShape("face", alien1X, alien1Y, Color::GREEN, 1.5);
      buff.drawScaleShape("neck", alien1X, alien1Y, Color::PURPLE, 1.5);
      buff.drawScaleShape("upperbody", alien1X, alien1Y, Color::PINK, 1.5);
      buff.drawScaleShape("lowerbody", alien1X, alien1Y, Color::PURPLE, 1.5);
      buff.drawScaleShape("eye", alien1X, alien1Y, Color::BLUE, 1.5);

      alien2X += 15;
      alien2X = (alien2X >= alienXmin)? (alien2X <= alienXmax)? alien2X : alienXmin : alienXmax;
      alien2Y += 15;
      alien2Y = (alien2Y >= alienYmin)? (alien2Y <= alienYmax)? alien2Y : alienYmin : alienYmax;
      buff.drawShape("cockpit", alien2X, alien2Y, Color::WHITE);
      buff.drawShape("face", alien2X, alien2Y, Color::GREEN);
      buff.drawShape("neck", alien2X, alien2Y, Color::PURPLE);
      buff.drawShape("upperbody", alien2X, alien2Y, Color::PINK);
      buff.drawShape("lowerbody", alien2X, alien2Y, Color::PURPLE);
      buff.drawShape("eye", alien2X, alien2Y, Color::BLUE);
      

      alien3X -= 25;
      alien3X = (alien3X >= alienXmin)? (alien3X <= alienXmax)? alien3X : alienXmin : alienXmax;
      alien3Y -= 15;
      alien3Y = (alien3Y >= alienYmin)? (alien3Y <= alienYmax)? alien3Y : alienYmin : alienYmax;
      buff.drawShape("cockpit", alien3X, alien3Y, Color::WHITE);
      buff.drawShape("face", alien3X, alien3Y, Color::GREEN);
      buff.drawShape("neck", alien3X, alien3Y, Color::PURPLE);
      buff.drawShape("upperbody", alien3X, alien3Y, Color::PINK);
      buff.drawShape("lowerbody", alien3X, alien3Y, Color::PURPLE);
      buff.drawShape("eye", alien3X, alien3Y, Color::BLUE);
      

      alien4X += 5;
      alien4X = (alien4X >= alienXmin)? (alien4X <= alienXmax)? alien4X : alienXmin : alienXmax;
      alien4Y += 45;
      alien4Y = (alien4Y >= alienYmin)? (alien4Y <= alienYmax)? alien4Y : alienYmin : alienYmax;
      buff.drawScaleShape("cockpit", alien4X, alien4Y, Color::WHITE, 2.3);
      buff.drawScaleShape("face", alien4X, alien4Y, Color::GREEN, 2.3);
      buff.drawScaleShape("neck", alien4X, alien4Y, Color::PURPLE, 2.3);
      buff.drawScaleShape("upperbody", alien4X, alien4Y, Color::PINK, 2.3);
      buff.drawScaleShape("lowerbody", alien4X, alien4Y, Color::PURPLE, 2.3);
      buff.drawScaleShape("eye", alien4X, alien4Y, Color::BLUE, 2.3);      

      alien5X -= 30;
      alien5X = (alien5X >= alienXmin)? (alien5X <= alienXmax)? alien5X : alienXmin : alienXmax;
      alien5Y += 20;
      alien5Y = (alien5Y >= alienYmin)? (alien5Y <= alienYmax)? alien5Y : alienYmin : alienYmax;
      buff.drawScaleShape("cockpit", alien5X, alien5Y, Color::WHITE, 2);
      buff.drawScaleShape("face", alien5X, alien5Y, Color::GREEN, 2);
      buff.drawScaleShape("neck", alien5X, alien5Y, Color::PURPLE, 2);
      buff.drawScaleShape("upperbody", alien5X, alien5Y, Color::PINK, 2);
      buff.drawScaleShape("lowerbody", alien5X, alien5Y, Color::PURPLE, 2);
      buff.drawScaleShape("eye", alien5X, alien5Y, Color::BLUE, 2);
      

      alien6X += 30;
      alien6X = (alien6X >= alienXmin)? (alien6X <= alienXmax)? alien6X : alienXmin : alienXmax;
      alien6Y -= 20;
      alien6Y = (alien6Y >= alienYmin)? (alien6Y <= alienYmax)? alien6Y : alienYmin : alienYmax;
      buff.drawScaleShape("cockpit", alien6X, alien6Y, Color::WHITE, 3);
      buff.drawScaleShape("face", alien6X, alien6Y, Color::GREEN, 3);
      buff.drawScaleShape("neck", alien6X, alien6Y, Color::PURPLE, 3);
      buff.drawScaleShape("upperbody", alien6X, alien6Y, Color::PINK, 3);
      buff.drawScaleShape("lowerbody", alien6X, alien6Y, Color::PURPLE, 3);
      buff.drawScaleShape("eye", alien6X, alien6Y, Color::BLUE, 3);
      
    }

    buff.drawShape("sys", sys1X, sys1Y, Color::RED);
    buff.drawShape("sys", sys2X, sys2Y, Color::ORANGE);
    buff.drawShape("sys", sys3X, sys3Y, Color::YELLOW);
    buff.drawShape("sys", sys4X, sys4Y, Color::GREEN);
    buff.drawShape("sys", sys5X, sys5Y, Color::BLUE);
    buff.drawShape("sys", sys6X, sys6Y, Color::PINK);
    buff.drawShape("sys", sys7X, sys7Y, Color::PURPLE);
    buff.drawScaleShape("sys", 1200, 550, Color::GRAY, 0.5);

    buff.drawShape("kirikanan", 0, 0, Color::NAVY);
    buff.drawShape("kirikanan", 1300, 0, Color::NAVY);
    buff.drawShape("kirikanan", 1350, 0, Color::NAVY);
    buff.drawShape("atasbawah", 0, 0, Color::NAVY);
    buff.drawShape("atasbawah", 0, 650, Color::NAVY);
    buff.drawShape("atasbawah", 0, 700, Color::NAVY);
    buff.drawShapeBorder("border", 50, 50, Color::WHITE);


    buff.drawShape("mouse", mousex, mousey, Color::WHITE);
    if (clickLeft) {
      buff.drawShape("mouse-click", mousex, mousey, Color::RED);
    }

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
	
      if (true) {
        if (mousex > sys1X && mousex < sys1X+100 && mousey > sys1Y && mousey < sys1Y+100) {
          buff.drawShapeBorder("sys", sys1X, sys1Y, Color::WHITE);
          if (clickLeft)
          system("./bin/1");
        }
        else if (mousex > sys2X && mousex < sys2X+100 && mousey > sys2Y && mousey < sys2Y+100) {
          buff.drawShapeBorder("sys", sys2X, sys2Y, Color::WHITE);
          if (clickLeft)
          system("./bin/2");
        }
        else if (mousex > sys3X && mousex < sys3X+100 && mousey > sys3Y && mousey < sys3Y+100) {
          buff.drawShapeBorder("sys", sys3X, sys3Y, Color::WHITE);
          if (clickLeft)
          system("./bin/3");
        }
        else if (mousex > sys4X && mousex < sys4X+100 && mousey > sys4Y && mousey < sys4Y+100) {
          buff.drawShapeBorder("sys", sys4X, sys4Y, Color::WHITE);
          if (clickLeft)
          system("./bin/4");
        }
        else if (mousex > sys5X && mousex < sys5X+100 && mousey > sys5Y && mousey < sys5Y+100) {
          buff.drawShapeBorder("sys", sys5X, sys5Y, Color::WHITE);
          if (clickLeft)
          system("./bin/5");
        }
        else if (mousex > sys6X && mousex < sys6X+100 && mousey > sys6Y && mousey < sys6Y+100) {
          buff.drawShapeBorder("sys", sys6X, sys6Y, Color::WHITE);
          if (clickLeft)
          system("./bin/6");
        }
        else if (mousex > sys7X && mousex < sys7X+100 && mousey > sys7Y && mousey < sys7Y+100) {
          buff.drawShapeBorder("sys", sys7X, sys7Y, Color::WHITE);
          if (clickLeft)
          system("./bin/7");
        }
        else {
          buff.drawScaleShapeBorder("sys", 1200, 550, Color::GRAY, 0.5);
          if (clickLeft)
          stop = true;
        }
      }
    }
    buff.apply();
  } while (!stop);

  return 0;
}
