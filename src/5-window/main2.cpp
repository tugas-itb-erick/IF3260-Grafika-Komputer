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
#define SCALECONSTANT 2
#define SCALECONSTANT2 0.5
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
  setConioTerminalNode();
  int bulletVisible = 0;
  
  Drawable plane = readFromFile("chars/4/pesawat.txt");
  // 
  //buff.addShape("ear", readFromFile("chars/4/Alien_Ear.txt"));
  //buff.addShape("scratch" , readFromFile("chars/4/Alien_Scratch.txt"));
  buff.addShape("eye", readFromFile("chars/4/Alien_Eye.txt"));
  buff.addShape("face", readFromFile("chars/4/Alien_Face.txt"));
  //buff.addShape("lip", readFromFile("chars/4/Alien_Lip.txt"));
  buff.addShape("cockpit" , readFromFile("chars/4/Ufo_Cockpit.txt"));
  buff.addShape("wheel3" , readFromFile("chars/4/Ufo_Wheel3.txt"));
  buff.addShape("wheel2" , readFromFile("chars/4/Ufo_Wheel2.txt"));
  buff.addShape("wheel1", readFromFile("chars/4/Ufo_Wheel1.txt"));
  buff.addShape("neck", readFromFile("chars/4/Ufo_Neck.txt"));
  buff.addShape("upperbody", readFromFile("chars/4/Ufo_UpperBody.txt"));
  buff.addShape("lowerbody", readFromFile("chars/4/Ufo_LowerBody.txt"));
  buff.translateAllShape(Point(400, 300));

  buff.addShape("platform", readFromFile("chars/4/Platform.txt"));
  buff.addShape("wall", readFromFile("chars/4/Tembok.txt"));
  buff.addShape("desk", readFromFile("chars/4/Meja.txt"));
  buff.addShape("panel", readFromFile("chars/4/Panel.txt"));
  buff.addShape("upButton", readFromFile("chars/4/Tombol_Atas.txt"));
  buff.addShape("downButton", readFromFile("chars/4/Tombol_Bawah.txt"));

  int zoomIn = 0;
  int zoomOut = 0;

  for (int time = 0; time < 1000; time += 5) {
    buff.reset();
    
    // GAMBAR TEMBOK DAN TOMBOL
  if (zoomIn) {
    buff.drawShape("upButton", 1, 1, Color::RED, false);
	//buff.scaleShape("ear", SCALECONSTANT);
	//buff.scaleShape("scratch" , SCALECONSTANT);
    buff.scaleShape("eye", SCALECONSTANT);
    //buff.scaleShape("lip", SCALECONSTANT);
    buff.scaleShape("face", SCALECONSTANT);
    buff.scaleShape("cockpit" , SCALECONSTANT);
    buff.scaleShape("wheel3" , SCALECONSTANT);
    buff.scaleShape("wheel2" , SCALECONSTANT);
    buff.scaleShape("wheel1", SCALECONSTANT);
    buff.scaleShape("upperbody", SCALECONSTANT);
    buff.scaleShape("neck", SCALECONSTANT);
    buff.scaleShape("lowerbody", SCALECONSTANT);
	}
	
	if (zoomOut) {
    buff.drawShape("downButton", 1, 1, Color::RED, false);
	//buff.scaleShape("ear", 1/SCALECONSTANT);
    //buff.scaleShape("scratch" , 1/SCALECONSTANT);
    buff.scaleShape("eye", SCALECONSTANT2);
    //buff.scaleShape("lip", 1/SCALECONSTANT);
    buff.scaleShape("face", SCALECONSTANT2);
    buff.scaleShape("cockpit" , SCALECONSTANT2);
    buff.scaleShape("wheel3" , SCALECONSTANT2);
    buff.scaleShape("wheel2" , SCALECONSTANT2);
    buff.scaleShape("wheel1", SCALECONSTANT2);
    buff.scaleShape("upperbody", SCALECONSTANT2);
    buff.scaleShape("neck", SCALECONSTANT2);
    buff.scaleShape("lowerbody", SCALECONSTANT2);
	}	
	
	if (time % 20 == 10) {
		buff.drawShape("panel", 1, 1, Color::GREEN, false);	
		buff.drawShape("upButton", 1, 1, Color::YELLOW, false);
		buff.drawShape("downButton", 1, 1, Color::YELLOW, false);
	} else {
		buff.drawShape("panel", 1, 1, Color::YELLOW, false);	
		buff.drawShape("upButton", 1, 1, Color::ORANGE, false);
		buff.drawShape("downButton", 1, 1, Color::ORANGE, false);
	}
	buff.drawShape("desk", 1, 1, Color::DARK_GREEN, false);
	buff.drawShape("wall", 1, 1, Color::GREY, false);	

  //buff.drawShape("scratch" , time, 0, Color::ORANGE);
  //buff.drawShape("ear", time, 0, Color::DARK_GREEN);
  buff.drawShape("eye", time, 0, Color::BLUE);
  //buff.drawShape("lip", time, 0, Color::RED);
  buff.drawShape("face", time, 0, Color::GREEN);
  buff.drawShape("cockpit" , time, 0, Color::WHITE);
  buff.drawShape("neck", time, 0, Color::PURPLE);
  buff.drawShape("upperbody", time, 0, Color::PINK);
  buff.drawShape("lowerbody", time, 0, Color::PURPLE);
  buff.drawShape("wheel3" , time, 0, Color::YELLOW);
  buff.drawShape("wheel2" , time, 0, Color::YELLOW);
  buff.drawShape("wheel1", time, 0, Color::YELLOW);
  
      
	
    // MEMBACA SPACEBAR
    if (kbhit()){
      int r;
      unsigned char c;
      if (r = read(0, &c, sizeof(c)) < 0) { //ngecek apakah tombol 0 s.d. 9 dipencet
        //do nothing
      } else { //selain 0 s.d. 9
      if (c == 'w' || c == 'W') { //keyboard w
			zoomIn = 1;
      zoomOut = 0;
		} else if (c == 's' || c == 'S') { //keyboard s
			zoomOut = 1;
      zoomIn = 0;
		} 
      }
    } else {
		zoomIn = 0;
		zoomOut = 0;
	}
    
    buff.apply();
  }

  system("clear");
	system("./bin/nub");
  return 0;
}
