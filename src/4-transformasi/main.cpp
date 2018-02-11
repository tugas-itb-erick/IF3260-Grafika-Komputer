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

int parabolaX(double initSpeed, double theta, double time) {
  return (int)(initSpeed * cos(theta*PI/180.0) * time + DOUBLE2INT_CORRECTION_VAL);
}

int parabolaY(double initSpeed, double theta, double time, double accel) {
  int sign = (time >= (initSpeed * sin(theta*PI/180.0) / accel)) ? 1 : -1;
  return (int)(initSpeed * sin(theta*PI/180.0) * time + sign * 0.5 * accel * pow(time, 2) + DOUBLE2INT_CORRECTION_VAL);
}

int freefall(double initSpeed, double time, double accel) {
  return (int)(initSpeed * time + 0.5 * accel * time * time + DOUBLE2INT_CORRECTION_VAL);
}


//----- MAIN PROGRAM -----//
int main() {
  Buffer buff;
  //setConioTerminalNode();

  Drawable plane = readFromFile("chars/4/Pesawat.txt");
  
  buff.addShape("plane", plane);
  buff.addShape("leftWindow", readFromFile("chars/4/jendela_pesawat_kiri.txt"));
  buff.addShape("rightWindow", readFromFile("chars/4/jendela_pesawat_kanan.txt"));
  buff.addShape("leftBlade" , readFromFile("chars/4/BalingBaling_Kiri.txt"));
  buff.addShape("rightBlade" , readFromFile("chars/4/BalingBaling_Kanan.txt"));
  buff.addShape("leftBladeLine" , readFromFile("chars/4/LineBaling_Kiri.txt"));
  buff.addShape("rightBladeLine" , readFromFile("chars/4/LineBaling_Kanan.txt"));
  buff.addShape("person", readFromFile("chars/4/Orang.txt"));
  buff.addShape("parachute", readFromFile("chars/4/Parasut.txt"));
  buff.addShape("leftWheel", readFromFile("chars/4/Roda_Kiri"));
  buff.addShape("rightWheel", readFromFile("chars/4/Roda_Kanan"));

  // Plane Initiation
  Point center = plane.centroid();
  center.negate();
  buff.translateAllShape(center + Buffer::CENTER);

  buff.addShape("platform", readFromFile("chars/4/Platform.txt"));
  buff.addShape("cannon", readFromFile("chars/4/Cannon.txt"));
  buff.addShape("ground", readFromFile("chars/4/Ground.txt"));
  buff.addShape("bullet", readFromFile("chars/4/Bullet.txt"));

  int initBulletX = 131;
  int initBulletY = 600;
  int deltaX, deltaY;
  int startNabrak = UNDEFINED;
  int loopCount = 0;
	double speedX = 100, speedY = -100;
	double timecol = 0;
  bool nabrak = false;
  for (double time = 0; time < 500; time += 0.5) {
    buff.reset();

    // Atur baling baling
    buff.drawShape("leftBlade", 0, 0, Color::YELLOW);
    buff.drawShape("rightBlade", 0, 0, Color::YELLOW);
    buff.drawShape("leftBladeLine", 0, 0, Color::YELLOW);
    buff.drawShape("rightBladeLine", 0, 0, Color::YELLOW);
    if (loopCount % 2 == 0) {
      buff.scaleShape("leftBlade", 2, Buffer::CENTER);
      buff.scaleShape("rightBlade", 2, Buffer::CENTER);
      buff.scaleShape("leftBladeLine", 2, Buffer::CENTER);
      buff.scaleShape("rightBladeLine", 2, Buffer::CENTER);
      buff.rotateShape("leftBlade", PI/2);
      buff.rotateShape("rightBlade", PI/2);
      buff.rotateShape("leftBladeLine", PI/2);
      buff.rotateShape("rightBladeLine", PI/2);
    }

    // Atur roda
    buff.drawShape("leftWheel", Color::GREEN);
    buff.drawShape("rightBlade", Color::GREEN);
    if (loopCount % 2 == 0) {
      buff.scaleShape("leftWheel", 2, Buffer::CENTER);
      buff.scaleShape("rightWheel", 2, Buffer::CENTER);
    }
    if (nabrak) {
      
    }

    // Atur orang dan parasut
    if (loopCount % 2 == 0) {
      buff.scaleShape("person", 2, Buffer::CENTER);
      buff.scaleShape("parachute", 2, Buffer::CENTER);
    }
    if (nabrak) {
      deltaX = parabolaX(-50, 60, time - startNabrak);
      deltaY = parabolaY(-100, 60, time - startNabrak, 15);
      buff.drawShape("person", deltaX, deltaY, Color::RED);
      if (time - startPerson > 5) {
        buff.drawShape("parachute", deltaX, deltaY, Color::RED);
      }
    }

    // Atur pesawat
    buff.drawShape("plane", 0, 0, Color::ORANGE);
    buff.drawShape("leftWindow", 0, 0, Color::WHITE);
    buff.drawShape("rightWindow", 0, 0, Color::WHITE);
    if (loopCount % 2 == 0) {
      buff.scaleShape("plane", 2, Buffer::CENTER);
      buff.scaleShape("leftWindow", 2, Buffer::CENTER);
      buff.scaleShape("rightWindow", 2, Buffer::CENTER);
      buff.centerShape("plane");
    }

    buff.drawShape("ground", 30, 670, Color::GREEN);
    buff.drawShape("platform", 30, 620, Color::BLUE);
    buff.drawShape("cannon", 81, 620, Color::PURPLE);

	/*int deltaX = parabolaX(speedX, 60, time);
	int deltaY = parabolaY(speedY, 60, time-timecol, 10);
    if ((initBulletY + deltaY) >= 650) {
    	speedY = speedY/2;
    	timecol = time;
    }
    buff.drawShape("bullet", initBulletX + deltaX, initBulletY + deltaY, Color::WHITE);*/

    if (time > 10) { /////////////////////////// HARUSNYA PAS PESAWATNYA KENA PELURU
      nabrak = true;
      if (startnabrak == UNDEFINED) startNabrak = time;
    }

    buff.apply();
    
    usleep(50);
    ++loopCount;
  }

  return 0;
}
