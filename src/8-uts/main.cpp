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
#include "models/CharDrawable.h"
using namespace std;


//----- CONSTANTS -----//
#define N_ALPHABETS 26
#define UNDEF 99999.99


//----- GLOBAL VARIABLES -----//
struct termios origTermios;
char name[] = "# AUDRY NYONATA#CATHERINE ALMIRA# DEWITA SONYA T.#  ERICK WIJAYA# KEZIA SUHENDRA# VEREN ILIANA K.#    WILLIAM####   THANK YOU...";
vector<pair<pair<int,int>,pair<int,int> > > line[30], pesawat;
int head, tail, height;
int hit = 0;
CharDrawable chars[N_ALPHABETS];
Buffer buff;


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

double gradient(int x1, int y1, int x2, int y2) {
    if (x1 - x2 == 0)
        return UNDEF;
    return (double) (y1-y2) / (x1-x2);
}

void drawPoint(int x, int y, int thickness, int red, int green, int blue, int a) {
    for (int i=x; i<x+thickness; i++) {
        for (int j=y; j<y+thickness; j++) {
            buff.drawPoint(i, j, Color(red, green, blue));
        }
    }
}

void drawLine(int x1, int y1, int x2, int y2, int thickness, int red, int green, int blue, int a = 1) {
    double m = gradient(x1, y1, x2, y2);
    int sign = (m < 0) ? -1 : 1;
    //cout << "gradient = " << m << endl;

    int dx, dy, e = 0;

  if (m >= -1 && m <= 1) {
        if (x1 > x2) {
            swap(x1, x2);
            swap(y1, y2);
        }
        dx = x2 - x1;
        dy = y2 - y1;
        int y = y1;

        for (int x = x1; x <= x2; x++) {
            drawPoint(x, y, thickness, red, green, blue, a);
            e += dy*sign;
            if (2*e >= dx) {
                y += sign;
                e -= dx;
            }
        }
    } else {
        if (y1 > y2) {
            swap(x1, x2);
            swap(y1, y2);
        }
        dx = x2 - x1;
        dy = y2 - y1;
        int x = x1;

        for (int y = y1; y <= y2; ++y) {
            drawPoint(x, y, thickness, red, green, blue, a);
            e += dx*sign;
            if (2*e >= dy) {
                x += sign;
                e -= dy;
            }
        }
    }
}

void printChar(char c, int hurufKe, int baris, int red, int green, int blue, int time) {
    int scale = 8, thickness = 4, top = 600, left = 300;
    int x1, y1, x2, y2;
    for (int i = 0; i < (int)line[c - 'A'].size(); ++i) {
    x1 = left + scale*line[c-'A'][i].first.first + (scale+1)*5*(hurufKe-1);
    y1 = top + scale*line[c-'A'][i].first.second + (scale+1)*7*(baris-1) - time;
    x2 = left + scale*line[c-'A'][i].second.first + (scale+1)*5*(hurufKe-1);
    y2 = top + scale*line[c-'A'][i].second.second + (scale+1)*7*(baris-1) - time;
    x1 = max(x1, 0); x1 = min(x1, (int)buff.getWidth());
    x2 = max(x2, 0); x2 = min(x2, (int)buff.getWidth());
    y1 = max(y1, 0); y1 = min(y1, (int)buff.getHeight());
    y2 = max(y2, 0); y2 = min(y2, (int)buff.getHeight());
    if (!(y1==0 && y2==0) && !(y1>buff.getHeight() && y2>y1>buff.getHeight())) drawLine(x1, y1, x2, y2, thickness, red, green, blue);
    }
}

void printPesawat(int time, int red, int green, int blue) {
  int scale = 5,thickness = 2,top = 300,left = 0;
  int x1, x2, y1, y2;
  tail = left + scale*pesawat[0].first.first + time;
  head = left + scale*pesawat[8].first.first + time;
  height = top + scale*19;
  for (int i=0;i<(int)pesawat.size();++i) {
    x1 = left + scale*pesawat[i].first.first + time;
    y1 = top + scale*pesawat[i].first.second;
    x2 = left + scale*pesawat[i].second.first + time;
    y2 = top + scale*pesawat[i].second.second;
    x1 = max(x1, 0); x1 = min(x1, (int)buff.getWidth());
    x2 = max(x2, 0); x2 = min(x2, (int)buff.getWidth());
    y1 = max(y1, 0); y1 = min(y1, (int)buff.getHeight());
    y2 = max(y2, 0); y2 = min(y2, (int)buff.getHeight());
    tail = min(tail,x1);
    head = max(head,x2);    
    if (!(x1==0 && x2==0) && !(x1>buff.getWidth() && x2>buff.getWidth())) {
      drawLine(x1, y1, x2, y2, thickness, red, green, blue);
    }
  }
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
  buff.reset();
  setConioTerminalNode();
  
  // Setup shapes
  buff.addShape("menu1", readShapeFromFile("chars/7/Menu1.txt"));
  buff.addShape("menu2", readShapeFromFile("chars/7/Menu2.txt"));
  buff.addShape("item", readShapeFromFile("chars/7/MenuItem.txt"));
  buff.addShape("checkbox", readShapeFromFile("chars/7/Checkbox.txt"));
  buff.addShape("checked", readShapeFromFile("chars/7/Checked.txt"));
  buff.addShape("small-box", readShapeFromFile("chars/7/Small.txt"));
  buff.addShape("big-box", readShapeFromFile("chars/7/Big.txt"));
  buff.addShape("jalan", readShapeFromFile("chars/7/jalan.txt"));
  buff.addShape("mouse", readShapeFromFile("chars/7/crosshair.txt"));
  buff.addShape("mouse-click", readShapeFromFile("chars/7/crosshair-click.txt"));

  // Setup layers
  buff.addLayer("gedung", readLayerFromFile("chars/7/gedung.txt"));
  buff.addLayer("jalan", readLayerFromFile("chars/7/jalan.txt"));
  buff.addLayer("lapangan", readLayerFromFile("chars/7/lapangan.txt"));
  
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

  // Mouse setup
  const char *pDevice = "/dev/input/mice";
  int fmouse = open(pDevice, O_RDONLY | O_NONBLOCK);
  if (fmouse == -1) {
    printf("Error opening %s\n", pDevice);
    return -1;
  }
  unsigned char event[3];
  int clickLeft = 0, clickRight;
  signed char relx, rely;
  int mousex = 700, mousey = 350;
  

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
  		buff.drawShapeFloodFill("menu2", xMenu2, yMenu2, treeColor);
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
    buff.drawShapeFloodFill("menu1", xMenu1, yMenu1, Color(50,50,50));
    buff.drawShapeFloodFill("item", xItem, yItem+diffItem*0, buildingColor); //checkbox building
    buff.drawShapeFloodFill("item", xItem, yItem+diffItem*1, streetColor); //checkbox street
    buff.drawShapeFloodFill("item", xItem, yItem+diffItem*2, treeColor); //checkbox tree
    buff.drawShapeFloodFill("item", xItem, yItem+diffItem*3, lapanganColor); //checkbox tree
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

    // Draw mouse
    buff.drawShapeScanline("mouse", mousex, mousey, Color::WHITE);
	if (clickLeft) {
		buff.drawShapeScanline("mouse-click", mousex, mousey, Color::RED);
	}
	
    buff.apply();	

    bool detectInput = false;
    for (int i=0;i<sizeof(event);++i) {
	  event[i] = 0;
    }
    input = 0;

    // Detect input from mouse or keyboard
    while (!detectInput) {
      if (kbhit()) {
        detectInput = true;
        read(0, &input, sizeof(input));
      }
      if (read(fmouse, event, sizeof(event))) {
        detectInput = 1;
      }
    }

    // Keyboard input
    if (input) {
      //read(0, &input, sizeof(input));
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
    }

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
		selectedMenu = 2;
		xSmall = mousex;
	    ySmall = mousey;
		if (xSmall < xminMove)
		  xSmall = xminMove;
		if (xSmall > xmaxMove)
		  xSmall = xmaxMove;
	    if (ySmall < yminMove)
		  ySmall = yminMove;
		if (ySmall > ymaxMove)
		  ySmall = ymaxMove;
      }
    }
  } while (input != 'q');


  // CREDIT 1
  /** BACA A s..d. Z **/
    FILE *fp;
    char c;

    for (c = 'A'; c <= 'Z'; c++) {
        char filename1[] = "chars/2/x.txt";
        filename1[8] = c;
        
        fp = fopen(filename1, "r");
        if (fp != NULL) {
            int x1, x2, y1, y2;
            while (fscanf(fp, "%d", &x1) == 1) {
        fscanf(fp, "%d", &y1);
        fscanf(fp, "%d", &x2);
        fscanf(fp, "%d", &y2);
                line[c - 'A'].push_back({{x1,y1},{x2, y2}});
            }
            fclose(fp);
        }
    }

    
    /** Baca pesawat **/
    fp = fopen("chars/2/pesawat.txt", "r");
  if (fp != NULL) {
    int x1, x2, y1, y2;
    while (fscanf(fp, "%d", &x1) == 1) {
      fscanf(fp, "%d", &y1);
      fscanf(fp, "%d", &x2);
      fscanf(fp, "%d", &y2);
      pesawat.push_back({{x1,y1},{x2, y2}});
    }
    fclose(fp);
  }


/** LOCAL VARIABEL **/   
    int r, g, b, trans;
    int time;
    trans = 0;
  
    int x,y,location;
  int garis1 = 0; int inc1 = 0;
  int garis2 = 0; int inc2 = 0;
  int garis3 = 0; int inc3 = 0;
  int countpeluru = 0;
  int durasipeluru = 200;
  int magicnumber = 5;
    for (time = 0; time < 1500; time+=magicnumber) {
      /* do some work */
      int counter = 0;
      int baris = 1;
      int hurufKe = 1;
      int startX;        
      
      buff.reset();

      // Cetak nama
      while (name[counter] != '\0') {
        if (name[counter]=='#') {
          hurufKe = 1;
          switch (baris) {
            case 1 : r=255; g=0; b=0; break;
            case 2 : r=255; g=127; b=0; break;
            case 3 : r=255; g=255; b=0; break;
            case 4 : r=0; g=255; b=0; break;
            case 5 : r=0; g=0; b=255; break;
            case 6 : r=127; g=0; b=255; break;
            case 7 : r=255; g=0; b=127; break;
            default : r=255; g=0; b=0; break;
          }
          baris++;
        } else {
          if (name[counter] != ' ') { 
            printChar(name[counter],hurufKe,baris,r,g,b,time);
          }
          hurufKe++;
        }
        counter++;
      }
      if (hit == 0) {
        printPesawat(5+time, 255, 255, 255);
      } else if (hit == 1) {
        //meletup
        drawLine(tail,height,head,height-75,35,255,0,0,0);
        drawLine(tail,height-75,head,height,35,255,0,0,0);
        drawLine(tail,height,head,height-75,25,255,127,0,0);
        drawLine(tail,height-75,head,height,25,255,127,0,0);
        drawLine(tail,height,head,height-75,15,255,255,0,0);
        drawLine(tail,height-75,head,height,15,255,255,0,0);
        printPesawat(5+time, 255, 225, 0);
      }

      //peluruu
      if (garis1 > 0) {
        int x1 = 700-inc1;
        int y1 = 750-inc1; 
        int x2 = 650-inc1;
        int y2 = 700-inc1;
        if (x1>15 && x1<buff.getWidth() & x2>15 && x2<buff.getWidth() && y1>15 && y1<buff.getHeight() && y2>15 && y2<buff.getHeight()){
          if ((x1>=tail && x1 <= head) && (y1<height || y2<height)) {
            hit++;
          } else {
            drawLine(x1,y1,x2,y2,3,255,255,255);
            drawLine(x1-15,y1-15,x2+15,y2+15,3,0,0,0,100);
          }
        }
        garis1--;
        inc1+=magicnumber+5;
      } else {
        inc1 = 0;
      }
      if (garis2 > 0) {
        int x1 = 700;
        int y1 = 750-inc2;
        int x2 = 700;
        int y2 = 700-inc2;
        if (x1>15 && x1<buff.getWidth() & x2>15 && x2<buff.getWidth() && y1>15 && y1<buff.getHeight() && y2>15 && y2<buff.getHeight()){
          if ((x1>=tail && x1 <= head) && (y1<height || y2<height)) {
            hit++;
          } else {
            drawLine(x1,y1,x2,y2,3,255,255,255);
            drawLine(x1,y1-15,x2,y2+15,3,0,0,0,100);
          }
        }
        garis2--;
        inc2+=magicnumber+5;
      } else {
        inc2 = 0;
      }
      if (garis3 > 0) {
        int x1 = 700+inc3;
        int y1 = 750-inc3; 
        int x2 = 750+inc3;
        int y2 = 700-inc3;
        if (x1>15 && x1<buff.getWidth() & x2>15 && x2<buff.getWidth() && y1>15 && y1<buff.getHeight() && y2>15 && y2<buff.getHeight()){
          if ((x1>=tail && x1 <= head) && (y1<height || y2<height)) {
            hit++;
          } else {
            drawLine(x1,y1,x2,y2,3,255,255,255);
            drawLine(x1+15,y1-15,x2-15,y2+15,3,0,0,0,100);
          }
        }
        garis3--;
        inc3+=magicnumber+5;
      } else {
        inc3 = 0;
      }
      delay(5);
//peluru22
    if (kbhit()){
      countpeluru++;
      int r;
      unsigned char c;
      if (r = read(0, &c, sizeof(c)) < 0) { //ngecek apakah tombol 0 s.d. 9 dipencet
        //do nothing
      } else { //selain 0 s.d. 9
        if (c == ' ') {
          if (countpeluru%3 == 1) {
            if (garis1>0){
              //do nothing
              //ga boleh muncul peluru lagi, tunggu sampe peluru ilang
            } else {
              garis1=durasipeluru;
              inc1 = 0;
            }
          } else if (countpeluru%3 == 2) {
            if (garis2>0){
              //do nothing
              //ga boleh muncul peluru lagi, tunggu sampe peluru ilang
            } else {
              garis2=durasipeluru;
              inc2 = 0;
            }
          } else if (countpeluru%3 == 0) {
            if (garis3>0){
              //do nothing
              //ga boleh muncul peluru lagi, tunggu sampe peluru ilang
            } else {
              garis3=durasipeluru;
              inc3 = 0;
            }
          }
        }
      }
    }  
        buff.apply();     
    }

  return 0;
}
