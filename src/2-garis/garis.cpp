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
#include <iostream> // std::cin, std::cout, std::endl
#include <algorithm> // std::min, std::max
#include <thread> //std::thread
using namespace std;

//----- CONSTANTS -----//
#define UNDEF 99999.99 // Large Positive Number

//----- GLOBAL VARIABLES -----//
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char *fbp = 0;

//----- FUNCTION DECLARATIONS -----//
double gradient(int x1, int y1, int x2, int y2);
void drawDot(int x, int y, int thickness = 1, int red = 255, int green = 255, int blue = 255, int a = 0);
void drawLine(int x1, int y1, int x2, int y2, int thickness = 1, int red = 255, int green = 255, int blue = 255, int a = 0);

//----- FUNCTION IMPLEMENTATIONS -----//
double gradient(int x1, int y1, int x2, int y2) {
    if (x1 - x2 == 0)
        return UNDEF;
    return (double) (y1-y2) / (x1-x2);
}

void drawDot(int x, int y, int thickness, int red, int green, int blue, int a) {
    long int location = 0;
    for (int i=x; i<x+thickness; i++) {
        for (int j=y; j<y+thickness; j++) {
            location = (i+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (j+vinfo.yoffset) * finfo.line_length;
            *(fbp + location) = blue;      // blue
	 		*(fbp + location + 1) = green; // green
	 		*(fbp + location + 2) = red;   // red
	 		*(fbp + location + 3) = a;     // transparency
        }
    }
}

void drawLine(int x1, int y1, int x2, int y2, int thickness, int red, int green, int blue, int a) {
    double m = gradient(x1, y1, x2, y2);
    int sign = (m < 0) ? -1 : 1;
    cout << "gradient = " << m << endl;

    // if (m >= -1 && m <= 1) {
    //     int small2Big = (x1 <= x2) ? 1 : -1;
    //     int small2BigY = (y1 <= y2) ? 1 : -1;
    //     int dx = (x2 - x1) * small2Big;
    //     int dy = (y2 - y1) * small2Big;
    //     int y = y1;
    //     int e = 0;

    //     for (int x = x1; x != x2; x += small2Big) {
    //         drawDot(x, y, thickness, red, green, blue, a); usleep(2000);
    //         e += dy * small2BigY;
    //         if (small2Big == 1) {
    //             if ((e << 1) >= dx) {
    //                 y += small2BigY;
    //                 e -= small2Big;
    //             }
    //         } else {
    //             if ((e << 1) < dx) {
    //                 y += small2BigY;
    //                 e -= small2Big;
    //             }
    //         }
    //     }

    // } else

    if (m >= 0 && m <= 1) {
        int dx, dy;
        int y = y1;
        int e = 0;

        if (min(x1, x2) == x1) {
            dx = x2 - x1;
            dy = y2 - y1;
            for (int x = x1; x <= x2; x++) {
                drawDot(x, y, thickness, red, green, blue, a); usleep(2000);
                e += dy;
                if ((e << 1) >= dx) {
                    y++;
                    e -= dx;
                }
            }
        } else {
            dx = x1 - x2;
            dy = y1 - y2;
            for (int x = x1; x >= x2; x--) {
                drawDot(x, y, thickness, red, green, blue, a); usleep(2000);
                e -= dy;
                if ((e << 1) < dx) {
                    y--;
                    e += dx;
                }
            }
        }
    } else if (m < 0 && m >= -1) {
        int dx, dy;
        int y = y1;
        int e = 0;

        if (min(x1, x2) == x1) {
            dx = x2 - x1;
            dy = y2 - y1;
            for (int x = x1; x <= x2; x++) {
                drawDot(x, y, thickness, red, green, blue, a); usleep(2000);
                e -= dy;
                if ((e << 1) >= dx) {
                    y--;
                    e -= dx;
                }
            }
        } else {
            dx = x1 - x2;
            dy = y1 - y2;
            for (int x = x1; x >= x2; x--) {
                drawDot(x, y, thickness, red, green, blue, a); usleep(2000);
                e += dy;
                if ((e << 1) < dx) {
                    y++;
                    e += dx;
                }
            }
        }
    }

    // } else if (m < 1) {

    // } else {
    //     int xstart = min(x1, x2);
    //     int xend = x1 + x2 - xstart;
    //     int x = xstart; int y = y1;
    //     while (x <= xend) {
    //         drawDot(x, y, thickness, red, green, blue, a);
    //         x++;
    //         y += sign;
    //     }
    // }
}

//----- MAIN PROGRAM -----//
int main()
{
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
    if (false) {//((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    drawLine(100, 100, 600, 400, 1);
    drawLine(600, 400, 100, 100, 1, 255, 0, 0);
    drawLine(100, 400, 600, 100, 1);
    drawLine(600, 100, 100, 400, 1, 255, 0, 0);

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
