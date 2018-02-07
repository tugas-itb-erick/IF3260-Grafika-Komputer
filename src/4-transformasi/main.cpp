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


//----- MAIN PROGRAM -----//
int main() {
  Buffer buff;

  for (int i=0; i<buff.getWidth(); i++) {
    for (int j=0; j<buff.getHeight(); j++) {
      buff[i][j] = Color::RED;
    }
  }

  buff.apply();


  return 0;
}
