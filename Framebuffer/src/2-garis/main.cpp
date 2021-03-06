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

#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <algorithm>
#include <sys/ioctl.h>
#include <time.h>
#include <iostream> // std::cin, std::cout, std::endl
#include <algorithm> // std::min, std::max
#include <thread> //std::thread
#include <vector>
#include "models/Point.h"
#include "models/Line.h"
#include "models/Color.h"
#include "models/CharDrawable.h"
using namespace std;

//----- CONSTANTS -----//
#define UNDEF 99999.99 // No longer need, bs pake Line::UNDEF
#define N_ALPHABETS 26

//----- GLOBAL VARIABLES -----//
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char *fbp = 0;
CharDrawable chars[N_ALPHABETS];

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

char name[] = "# AUDRY NYONATA#CATHERINE ALMIRA# DEWITA SONYA T.#  ERICK WIJAYA# KEZIA SUHENDRA# VEREN ILIANA K.#    WILLIAM####   THANK YOU...";
vector<pair<pair<int,int>,pair<int,int> > > line[30], pesawat;
int head, tail, height;
int hit = 0;
//----- FUNCTION DECLARATIONS -----//
double gradient(int x1, int y1, int x2, int y2); // No longer need, bs pake Line.gradient()
// Calculate gradient from a Line
// TODO: ganti x1 y1 jadi Line L, atau const Line& L

void drawPoint(int x, int y, int thickness = 1, int red = 255, int green = 255, int blue = 255, int a = 0);
// Draw a single Point on the screen
// TODO: ganti x y jadi Point P, buang red green blue a

void drawLine(int x1, int y1, int x2, int y2, int thickness = 1, int red = 255, int green = 255, int blue = 255, int a = 0);
// Draw a single line from start point to end point (direction matters)
// TODO: ganti x1 dll jdi Line, buang red green blue a

//----- FUNCTION IMPLEMENTATIONS -----//
double gradient(int x1, int y1, int x2, int y2) {
    if (x1 - x2 == 0)
        return UNDEF;
    return (double) (y1-y2) / (x1-x2);
}

void drawPoint(int x, int y, int thickness, int red, int green, int blue, int a) {
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
		x1 = max(x1, 0); x1 = min(x1, (int)vinfo.xres);
		x2 = max(x2, 0); x2 = min(x2, (int)vinfo.xres);
		y1 = max(y1, 0); y1 = min(y1, (int)vinfo.yres - 16);
		y2 = max(y2, 0); y2 = min(y2, (int)vinfo.yres - 16);
		if (!(y1==0 && y2==0) && !(y1>vinfo.yres-20 && y2>vinfo.yres-20)) drawLine(x1, y1, x2, y2, thickness, red, green, blue);
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
		x1 = max(x1, 0); x1 = min(x1, (int)vinfo.xres);
		x2 = max(x2, 0); x2 = min(x2, (int)vinfo.xres);
		y1 = max(y1, 0); y1 = min(y1, (int)vinfo.yres - 16);
		y2 = max(y2, 0); y2 = min(y2, (int)vinfo.yres - 16);
		tail = min(tail,x1);
		head = max(head,x2);		
		if (!(x1==0 && x2==0) && !(x1>vinfo.xres && x2>vinfo.xres)) {
			drawLine(x1, y1, x2, y2, thickness, red, green, blue);
		}
	}
}


//----- UNTUK KEY INTTERUPT -------//

struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
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
	
    for (time = 0; time < 1500; time++) {
			/* do some work */
			int counter = 0;
			int baris = 1;
			int hurufKe = 1;
			int startX;        
			
			/** background fullscreeeeeennnn **/
			for (y = 0; y < vinfo.yres-10; y++) {
				for (x = 0; x < vinfo.xres; x++) {
					location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
							   (y+vinfo.yoffset) * finfo.line_length;
				//array RGB + transparenlinecy. 255 255 255 0 = putih
				if (vinfo.bits_per_pixel == 32) {
						*(fbp + location) = 0;        // Some blue
						*(fbp + location + 1) = 0;    // A little green
						*(fbp + location + 2) = 0;    // A lot of red
						*(fbp + location + 3) = 100;    // No transparency
					}
				}
			}

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
				delay(100);
				drawLine(tail,height,head,height-75,25,255,127,0,0);
				drawLine(tail,height-75,head,height,25,255,127,0,0);
				delay(100);
				drawLine(tail,height,head,height-75,15,255,255,0,0);
				drawLine(tail,height-75,head,height,15,255,255,0,0);
				delay(100);
				printPesawat(5+time, 255, 225, 0);
			}

			//peluruu
			if (garis1 > 0) {
				int x1 = 700-inc1;
				int y1 = 750-inc1; 
				int x2 = 650-inc1;
				int y2 = 700-inc1;
				if (x1>15 && x1<vinfo.xres-25 & x2>15 && x2<vinfo.xres-25 && y1>15 && y1<vinfo.yres-25 && y2>15 && y2<vinfo.yres-25){
					if ((x1>=tail && x1 <= head) && (y1<height || y2<height)) {
						hit++;
					} else {
						drawLine(x1,y1,x2,y2,3,255,255,255);
						drawLine(x1-15,y1-15,x2+15,y2+15,3,0,0,0,100);
					}
				}
				garis1--;
				inc1+=5;
			} else {
				inc1 = 0;
			}
			if (garis2 > 0) {
				int x1 = 700;
				int y1 = 750-inc2;
				int x2 = 700;
				int y2 = 700-inc2;
				if (x1>15 && x1<vinfo.xres-25 & x2>15 && x2<vinfo.xres-25 && y1>15 && y1<vinfo.yres-25 && y2>15 && y2<vinfo.yres-25){
					if ((x1>=tail && x1 <= head) && (y1<height || y2<height)) {
						hit++;
					} else {
						drawLine(x1,y1,x2,y2,3,255,255,255);
						drawLine(x1,y1-15,x2,y2+15,3,0,0,0,100);
					}
				}
				garis2--;
				inc2+=5;
			} else {
				inc2 = 0;
			}
			if (garis3 > 0) {
				int x1 = 700+inc3;
				int y1 = 750-inc3; 
				int x2 = 750+inc3;
				int y2 = 700-inc3;
				if (x1>15 && x1<vinfo.xres-25 & x2>15 && x2<vinfo.xres-25 && y1>15 && y1<vinfo.yres-25 && y2>15 && y2<vinfo.yres-25){
					if ((x1>=tail && x1 <= head) && (y1<height || y2<height)) {
						hit++;
					} else {
						drawLine(x1,y1,x2,y2,3,255,255,255);
						drawLine(x1+15,y1-15,x2-15,y2+15,3,0,0,0,100);
					}
				}
				garis3--;
				inc3+=5;
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
    }
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
