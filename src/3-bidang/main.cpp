/*
13515021 - Dewita Sonya Tarabunga
13515057 - Erick Wijaya
13515063 - Kezia Suhendra
13515078 - Veren Iliana
13515087 - Audry Nyonata
13515111 - Catherine Almira
13515144 - William
*/

/*
To test that the Linux framebuffer is set up correctly, and that the device permissions
are correct, use the program below which opens the frame buffer and draws a gradient-
filled red square:

retrieved from:
Testing the Linux Framebuffer for Qtopia Core (qt4-x11-4.2.2)

http://cep.xor.aps.anl.gov/software/qt4-x11-4.2.2/qtopiacore-testingframebuffer.html
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <iostream>
#include <algorithm>
#include <thread>
#include <vector>
#include <queue>
#include "models/Color.h"
#include "models/Point.h"
#include "models/Line.h"
#include "models/Triangle.h"
#include "models/CharDrawable.h"
using namespace std;

//----- CONSTANTS -----//
#define N_ALPHABETS 26
#define DELETE 8
#define BACKSPACE 127

//----- GLOBAL VARIABLES -----//
struct termios origTermios;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char *fbp = 0;
vector<CharDrawable> chars;


//----- FUNCTION DECLARATIONS -----//
void initChars();
void drawPoint(Point P, Color cl = Color::WHITE, int thickness = 1);
void drawLine(Line L, Color cl = Color::WHITE, int thickness = 1);
void drawChar(char c, int x, int y, Color cl = Color::WHITE); // vector<CharDrawable> chars must be initialized
void drawChar(CharDrawable c, int x, int y, Color cl = Color::WHITE);
void floodFill(Triangle T, Color cl = Color::WHITE);
void delay(int numOfSeconds);
void resetTerminalMode();
void setConioTerminalMode();
int kbhit();


//----- FUNCTION IMPLEMENTATIONS -----//
void initChars() {
    FILE *fp;
    char c;

    for (c = 'A'; c <= 'Z'; c++) {
        char filename[] = "chars/x.txt";
        filename[6] = c;
        
        fp = fopen(filename, "r");
        if (fp != NULL) {
            // Buat CharDrawable, lalu push ke vector chars
            fclose(fp);
        }
    }

    // Baca file utk karakter selain A-Z bila ada


}

void drawPoint(Point P, Color cl, int thickness) {
    long int location = 0;
    for (int i=P.x; i<P.x+thickness; i++) {
        for (int j=P.y; j<P.y+thickness; j++) {
            location = (i+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (j+vinfo.yoffset) * finfo.line_length;
            *(fbp + location) = cl.b;     // blue
	 		*(fbp + location + 1) = cl.g; // green
	 		*(fbp + location + 2) = cl.r; // red
	 		*(fbp + location + 3) = cl.a; // transparency
        }
    }
}

void drawLine(Line L, Color cl, int thickness) {
    int x1 = L.first.x,
        x2 = L.second.x,
        y1 = L.first.y,
        y2 = L.second.y;
    double m = L.gradient();
    int sign = (m < 0) ? -1 : 1;

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
            drawPoint(Point(x, y), cl, thickness);
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
            drawPoint(Point(x, y), cl, thickness);
            e += dx*sign;
            if (2*e >= dy) {
                x += sign;
                e -= dy;
            }
        }
    }
}

void drawChar(char c, int x, int y, Color cl) {
    bool found = false;
    for(int i=0; i<chars.size(); i++) {
        if (c == chars[i].letter) {
            found = true;
            drawChar(chars[i], x, y, cl);
        }
    }
    if (!found) {
        cout << "The character \'" << c << "\' has not been initialized in vector chars!" << endl;
    }
}

void drawChar(CharDrawable c, int x, int y, Color cl) {
    for(int i=0; i<c.triangles.size(); i++) {
        c.triangles[i].first.x += x;
        c.triangles[i].second.x += x;
        c.triangles[i].third.x += x;
        c.triangles[i].first.y += y;
        c.triangles[i].second.y += y;
        c.triangles[i].third.y += y;
        floodFill(c.triangles[i], cl);
    }
}

void floodFill(Triangle T, Color cl) {
    Point center = T.centroid();
    queue<Point> q;
    q.push(center);

    // MASIH BOROS MEMORY
    bool visited[2000][1000]; // PERLU DIGANTI JD UKURAN [maxX-minX+1][maxY-minY+1]
    for (int i=0; i<2000; i++) {
        for (int j=0; j<1000; j++) {
            visited[i][j] = false;
        }
    }

    while (!q.empty()) {
        Point c = q.front();
        q.pop();

        // if (c.x-minX < 0 || c.y-minY < 0 || c.x-minX >= maxX-minX+1 || 
        // c.y-minY >= maxY-minY+1 || visited[c.x][c.y] || !T.hasPoint(c)) {
        if (c.x < 0 || c.y < 0 || visited[c.x][c.y] || !T.hasPoint(c)) {
            // Do Nothing
        } else {
            visited[c.x][c.y] = true; // visited[c.x-minX][c.y-minY] = true;
            drawPoint(c, cl);

            q.push(Point(c.x, c.y-1));
            q.push(Point(c.x-1, c.y));
            q.push(Point(c.x+1, c.y));
            q.push(Point(c.x, c.y+1));
        }
    }
}

void delay(int numOfSeconds) {
    // Converting time into milli_seconds
    int milliSeconds = 1000 * numOfSeconds;
 
    // Stroing start time
    clock_t startTime = clock();
 
    // looping till required time is not acheived
    while (clock() < startTime + milliSeconds) {
        
	}
}

void resetTerminalMode() {
    tcsetattr(0, TCSANOW, &origTermios);
}

void setConioTerminalMode() {
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
    int fbfd = 0;
    long int screensize = 0;

    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (false) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    //--------------------------------------------------------------------------------------//
    //--------------------------------------------------------------------------------------//

    setConioTerminalMode();

    initChars(); // Baca File Eksternal, belum diimplementasi

    // Contoh Instance CharDrawable huruf L
    char karakterL = 'L';
    vector<Point> titikL;
    titikL.push_back(Point(0,0));
    titikL.push_back(Point(200,0));
    titikL.push_back(Point(0,400));
    titikL.push_back(Point(200,200));
    titikL.push_back(Point(400,200));
    titikL.push_back(Point(400,400));
    vector<Triangle> segitigaL;
    segitigaL.push_back(Triangle(titikL[3], titikL[4], titikL[5]));
    segitigaL.push_back(Triangle(titikL[2], titikL[3], titikL[5]));
    segitigaL.push_back(Triangle(titikL[0], titikL[2], titikL[3]));
    segitigaL.push_back(Triangle(titikL[0], titikL[1], titikL[3]));

    chars.push_back(CharDrawable(karakterL, titikL, segitigaL));
    drawChar('L', 100, 100, Color::BLUE); // FLOODFILLNYA MASI BOROS MEMORY KRN KOTAKNYA MASI 2000x1000
    drawChar('A', 200, 200, Color::PURPLE);

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
