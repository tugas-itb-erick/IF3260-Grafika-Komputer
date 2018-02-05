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
#include "models/Drawable.h"
using namespace std;

//----- CONSTANTS -----//
#define N_ALPHABETS 26
#define DELETE 8
#define BACKSPACE 127
#define ESC 27
#define SPACE 32

//----- GLOBAL VARIABLES -----//
struct termios origTermios;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char *fbp = 0;
char name[] = "# AUDRY NYONATA#CATHERINE ALMIRA# DEWITA SONYA T.#  ERICK WIJAYA# KEZIA SUHENDRA# VEREN ILIANA K.#    WILLIAM";
vector<vector<Color> > buffer;
Point dp[] = {Point(0, 1), Point(-1, 0), Point(0, -1), Point(1, 0)};
Drawable plane;
const Point CENTER = Point(680, 350);


//----- FUNCTION DECLARATIONS -----//
void init();
void resetBuffer();
void initBuffer(int row, int col);
void printBuffer(int row, int col);
void drawPoint(Point P, Color cl = Color::WHITE, int thickness = 1);
void drawLine(Line L, Color cl = Color::WHITE, int thickness = 1);
bool canDraw(Triangle t);
void drawTriangle(Triangle t, Color cl);
void draw(Drawable& d, int x, int y, Color cl = Color::WHITE);
void floodFill(Triangle T, Color cl = Color::WHITE);
void delay(int numOfSeconds);
void scale(Drawable& d, int k);
void translate(Drawable& d, const Point& p);
void center(Drawable& d);

//----- FUNCTION IMPLEMENTATIONS -----//
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

void resetBuffer() {
    for (auto& x : buffer) {
        for (auto& y : x) {
            y = Color::BLACK;
        }
    }
}

void initBuffer(int row, int col) {
    buffer.resize(row);
    for (int i = 0; i < row; ++i) {
        buffer[i].resize(col);
    }
    resetBuffer();
}

void printBuffer(int row, int col) {
    int x, y, location = 0;
    for (x = 0; x < row; x++) {
        for (y = 0; y < col; y++) {
        location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
        
        if (vinfo.bits_per_pixel == 32) {
                *(fbp + location) = buffer[x][y].b;
                *(fbp + location + 1) = buffer[x][y].g;
                *(fbp + location + 2) = buffer[x][y].r;
                *(fbp + location + 3) = buffer[x][y].a;
            }
        }
    }
}

void drawPoint(Point P, Color cl, int thickness) {
    long int location = 0;
    for (int i=P.x; i<P.x+thickness; i++) {
        for (int j=P.y; j<P.y+thickness; j++) {
			if (i && j && i < buffer.size() && j < buffer[i].size() && buffer[i][j] == Color::BLACK) {
				buffer[i][j] = cl;
			}
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

void drawTriangle(Triangle t, Color cl) {
    drawLine(Line(t.first, t.second), cl);
    drawLine(Line(t.first, t.third), cl);
    drawLine(Line(t.third, t.second), cl);
}

bool canDraw(Triangle t) {
    if (t.first.y < 5 || t.first.y > vinfo.yres - 10) return false;
    if (t.second.y < 5 || t.second.y > vinfo.yres - 10) return false;
    if (t.third.y < 5 || t.third.y > vinfo.yres - 10) return false;
    if (t.first.x < 5 || t.first.x > vinfo.xres - 10) return false;
    if (t.second.x < 5 || t.second.x > vinfo.xres - 10) return false;
    if (t.third.x < 5 || t.third.x > vinfo.xres - 10) return false;
    return true;
}

void draw(Drawable& d, int x, int y, Color cl) {
    for(int i=0; i< d.triangles.size(); i++) {
        if (canDraw(d.triangles[i] + Point(x, y))) {
    		drawTriangle(d.triangles[i] + Point(x, y), cl);
    		floodFill(d.triangles[i] + Point(x, y), cl);
        }
    }
}

void floodFill(Triangle T, Color cl) {
    Point center = T.centroid();
    queue<Point> q;
    q.push(center);

    drawPoint(center, cl);

    while (!q.empty()) {
        Point c = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            Point next = c + dp[i];
            if (buffer[next.x][next.y] == Color::BLACK) {
                drawPoint(next, cl);
                q.push(next);
            }
        }
    }
}

void scale(Drawable& d, double k) {
    for (auto& e : d.points) {
        e *= k;
    }
    for (auto& e : d.triangles) {
        e.first *= k;
        e.second *= k;
        e.third *= k;
    }
}

void translate(Drawable& d, const Point& p) {
    for (auto& e : d.points) {
        e += p;
    }
    for (auto& e : d.triangles) {
        e.first += p;
        e.second += p;
        e.third += p;
    }
}

void center(Drawable& d) { ///// MASI KURANG TENGAH
    Point cen = d.centroid();
    cen.negate();
    cen += CENTER;
    translate(d, cen);
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

    init();
    initBuffer(vinfo.xres, vinfo.yres);

    for (int time = 0; time < 200; time++) {
        if (time % 15 == 0) {
            resetBuffer();
            draw(plane, 0, 0, Color::RED);
            scale(plane, 1.08);
            center(plane);
            printBuffer(vinfo.xres, vinfo.yres - 25);
        }
    }
    
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
