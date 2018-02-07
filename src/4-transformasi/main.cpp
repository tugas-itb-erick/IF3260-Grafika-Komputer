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


//----- GLOBAL VARIABLES -----//
struct termios origTermios;


//----- FUNCTION DECLARATIONS -----//
Drawable readFromFile(const string& filename);
int parabolaX(double initSpeed, double theta, double time);
int parabolaY(double initSpeed, double theta, double time, double accel);
int freefall(double initSpeed, double time, double gravity);
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
    int nPoint, nTriangle;
    vector<Point> vp;
    vector<Triangle> vt;

    // Read points
    infile >> nPoint;
    for (int i=0; i<nPoint; i++) {
      int x, y;
      infile >> x >> y;
      vp.push_back(Point(x, y));
    }

    // Read triangles
    infile >> nTriangle;
    for (int i=0; i<nTriangle; i++) {
      int a, b, c;
      infile >> a >> b >> c;
      vt.push_back(Triangle(vp[a], vp[b], vp[c]));
    }

    infile.close();
    return Drawable(vp, vt);
  }
  return Drawable();
}

int parabolaX(double initSpeed, double theta, double time) {
  return (int)(initSpeed * cos(theta*PI/180.0) * time + DOUBLE2INT_CORRECTION_VAL);
}

int parabolaY(double initSpeed, double theta, double time, double accel) {
  return (int)(initSpeed * sin(theta*PI/180.0) * time - 0.5 * accel * time * time + DOUBLE2INT_CORRECTION_VAL);
}

int freefall(double initSpeed, double time, double gravity) {
  return (int)(initSpeed * time + 0.5 * gravity * time * time + DOUBLE2INT_CORRECTION_VAL);
}


//----- MAIN PROGRAM -----//
int main() {
  Buffer buff;
  setConioTerminalNode();
  
  buff.addShape("plane", readFromFile("chars/4/Pesawat.txt"));
  // buff.addShape("body", body);
  // buff.addShape("wing-l", wingL);
  // buff.addShape("wing-r", wingR);
  // buff.addShape("blades-l", bladesL);
  // buff.addShape("blades-r", bladesR);
  // buff.addShape("wheel-l", wheelL);
  // buff.addShape("wheel-r", wheelR);
  buff.addShape("platform", readFromFile("chars/4/Platform.txt"));
  buff.addShape("cannon", readFromFile("chars/4/Cannon.txt"));
  buff.addShape("ground", readFromFile("chars/4/Ground.txt"));

  for (int time = 0; time < 300; time++) {
    if (time % 30 == 0) {
      buff.reset();
      buff.drawShape("plane", 0, 0, Color::RED);
      buff.scaleShape("plane", 1.08);
      buff.centerShape("plane");
      buff.apply();
    }
  }

  return 0;
}
