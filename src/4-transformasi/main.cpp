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
#include "models/Color.h"
#include "models/Buffer.h"
using namespace std;

//----- GLOBAL VARIABLES -----//
struct termios origTermios;
Drawable plane;


//----- FUNCTION DECLARATIONS -----//
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

void init() {
    FILE *fp;
    char c;

    char filename[] = "chars/4/Pesawat.txt"; // ISINYA MASI HURUF O
    
    fp = fopen(filename, "r");
    if (fp != NULL) {
        int nPoint, nTriangle;
        vector<Point> vp;
        vector<Triangle> vt;

        // Read points
        fscanf(fp, "%d", &nPoint);
        for (int i=0; i<nPoint; i++) {
            int x, y;
            fscanf(fp, "%d", &x);
            fscanf(fp, "%d", &y);
            vp.push_back(Point(x, y));
        }

        // Read triangles
        fscanf(fp, "%d", &nTriangle);
        for (int i=0; i<nTriangle; i++) {
            int a, b, c;
            fscanf(fp, "%d", &a);
            fscanf(fp, "%d", &b);
            fscanf(fp, "%d", &c);
            vt.push_back(Triangle(vp[a], vp[b], vp[c]));
        }

        plane = Drawable(vp, vt);
        fclose(fp);
    }

}

//----- MAIN PROGRAM -----//
int main() {
  init();
  Buffer buff;

  setConioTerminalNode();
  
  buff.addShape("plane", plane);

  // buff.addShape("body", body);
  // buff.addShape("wing-l", wingL);
  // buff.addShape("wing-r", wingR);
  // buff.addShape("blades-l", bladesL);
  // buff.addShape("blades-r", bladesR);
  // buff.addShape("wheel-l", wheelL);
  // buff.addShape("wheel-r", wheelR);
  // buff.addShape("cannon", cannon);

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
