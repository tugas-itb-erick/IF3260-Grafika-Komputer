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

//----- CONSTANTS -----//
#define UNDEF 99999.99 // Large Positive Number

//----- GLOBAL VARIABLES -----//
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char *fbp = 0;
long int location = 0;

//----- FUNCTION DECLARATIONS -----//
double gradient(int x1, int y1, int x2, int y2);
void drawDot(int x, int y, int thickness);
void drawLine(int x1, int y1, int x2, int y2, int thickness = 1);

//----- FUNCTION IMPLEMENTATIONS -----//
double gradient(int x1, int y1, int x2, int y2) {
    if (x1 - x2 == 0)
        return UNDEF;
    return (double) (y1-y2) / (x1-x2);
}

void drawDot(int x, int y, int thickness) {
    // pake yg kmarin
}

void drawLine(int x1, int y1, int x2, int y2, int thickness) {
    // https://www.youtube.com/watch?v=345dAMp4RsI
    // https://www.youtube.com/watch?v=m2enpQNSYEY
    // m < 1, find y, x+=1
    // m > 1, find x, y+=1
    // m = 1, x+=1, y+=1
    double m = gradient(x1, y1, x2, y2);
    bool negative = m < 0;
    if (negative) {
        m = -m;
    }
    int initPk = 2*(y2-y1) - (x2-x1);

    if (m == UNDEF) {

    } else if (m > 1) {

    } else if (m < 1) {

    } else {

    }
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

    
    
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
