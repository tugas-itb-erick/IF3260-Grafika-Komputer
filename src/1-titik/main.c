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

#define N_ALPHABETS 26
#define HEIGHT_CHAR 7
#define WIDTH_CHAR 5
#define PIXEL_SIZE 5

char name[] = "# AUDRY NYONATA#CATHERINE ALMIRA# DEWITA SONYA T.#  ERICK WIJAYA# KEZIA SUHENDRA# VEREN ILIANA K.#    WILLIAM#####   THANK YOU...";

char alphabets[N_ALPHABETS][HEIGHT_CHAR][WIDTH_CHAR];
char space[HEIGHT_CHAR][WIDTH_CHAR];
char dot[HEIGHT_CHAR][WIDTH_CHAR];

int i, j, x, y;
int top = 700;
int left = 450;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int location;
char *fbp = 0;

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

void cetak(char c, int line, int hurufKe, int red, int green, int blue, int transparency, int time) {
	//HURUF KE-1 DI INDEKS 0, LINE PERTAMA DI INDEKS 0
	hurufKe--;
	line--;
	
	int i, j;
	for (i=0;i<HEIGHT_CHAR;i++) {
		for (j=0; j<WIDTH_CHAR; j++) {
			char karakter;
			if (c == ' '){
				karakter = space[i][j];
			} else if (c == '.'){
				karakter = dot[i][j];
			} else {
				karakter = alphabets[c-'A'][i][j];
			}			
			
			if (karakter=='1') {
				 for (x	 = 0; x < PIXEL_SIZE; x++) {
					 for (y = 0; y < PIXEL_SIZE; y++) {
						 // WIDTH_CHAR + 1 = 5 + JARAK ANTAR HURUF (BUKAN JARAK ANTAR KATA)
						 location = (left+x+(hurufKe*(WIDTH_CHAR+1)*PIXEL_SIZE)+(PIXEL_SIZE*j+1)+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
									(top-time+y+(line*(HEIGHT_CHAR+2)*PIXEL_SIZE)+(PIXEL_SIZE*i+1)+vinfo.yoffset) * finfo.line_length;
						
						if ((top-time+y+(line*(HEIGHT_CHAR+2)*PIXEL_SIZE)+(PIXEL_SIZE*i+1))< 0) {
							//BIAR GA SEGFAULT
						} else if ((top-time+y+(line*(HEIGHT_CHAR+2)*PIXEL_SIZE)+(PIXEL_SIZE*i+1)) > vinfo.yres-16)  {
							//BIAR GA SEGFAULT							
						} else {
							 if (vinfo.bits_per_pixel == 32) {
								 *(fbp + location) = blue; //Blue
								 *(fbp + location + 1) = green; //Green
								 *(fbp + location + 2) = red; //Red
								 *(fbp + location + 3) = transparency; //Transparency
							 }
						}
					 }
				 }
			 }
		 }
	 }
}

int main()
{
    int fbfd = 0;
    long int screensize = 0;
    x = 0;
    y = 0;
	location = 0;

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
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");


	/** BACA A s..d. Z **/
	FILE *fp;
	char c;

    for (c = 'A'; c <= 'Z'; c++) {
        char filename1[] = "chars/1/x.txt";
        filename1[8] = c;
        
        fp = fopen(filename1, "r");
        if (fp != NULL) {
            for (i=0; i<HEIGHT_CHAR; i++){
                for (j=0; j<WIDTH_CHAR; j++){
                    alphabets[c - 'A'][i][j] = fgetc(fp);
                    //printf("%c", alphabets[c-'A'][i][j]);
                }
                fgetc(fp);
                //printf("\n");
            }
            fclose(fp);
        }
    }
    
    /** BACA SPACE & DOT **/
	char filename2[] = "chars/1/space.txt";
	fp = fopen(filename2, "r");
	if (fp != NULL) {
		for (i=0; i<HEIGHT_CHAR; i++){
			for (j=0; j<WIDTH_CHAR; j++){
				space[i][j] = fgetc(fp);
				//printf("%c", space[i][j]);
			}
			fgetc(fp);
			//printf("\n");
		}
		fclose(fp);
	}

	char filename3[] = "chars/1/dot.txt";
	fp = fopen(filename3, "r");
	if (fp != NULL) {
		for (i=0; i<HEIGHT_CHAR; i++){
			for (j=0; j<WIDTH_CHAR; j++){
				dot[i][j] = fgetc(fp);
				//printf("%c", dot[i][j]);
			}
			fgetc(fp);
			//printf("\n");
		}
		fclose(fp);
	}

    // sabar biar keliatan
	delay(1000);
	
	int r, g, b, trans;
	int time;
	trans = 0;
	
	// for (y = 0; y < vinfo.yres-15; y++) {
	// 	for (x = 0; x < vinfo.xres; x++) {
	// 		location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
	// 				   (y+vinfo.yoffset) * finfo.line_length;
	// 	//array RGB + transparency. 255 255 255 0 = putih
	// 	if (vinfo.bits_per_pixel == 32) {
	// 			*(fbp + location) = 0;        // Some blue
	// 			*(fbp + location + 1) = 0;    // A little green
	// 			*(fbp + location + 2) = 0;    // A lot of red
	// 			*(fbp + location + 3) = 100;    // No transparency
	// 		}
	// 	}
	// }

	for (time = 0; time < 1500; time++) {
		int counter = 0;
		int baris = 1;
		int hurufKe = 1;
		
		/** background fullscreeeeeennnn **/
		for (y = 0; y < vinfo.yres-15; y++) {
			for (x = 0; x < vinfo.xres; x++) {
				location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
						   (y+vinfo.yoffset) * finfo.line_length;
			//array RGB + transparency. 255 255 255 0 = putih
			if (vinfo.bits_per_pixel == 32) {
					*(fbp + location) = 0;        // Some blue
					*(fbp + location + 1) = 0;    // A little green
					*(fbp + location + 2) = 0;    // A lot of red
					*(fbp + location + 3) = 100;    // No transparency
				}
			}
		}
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
				cetak(name[counter],baris,hurufKe,r,g,b,trans, time);
				hurufKe++;
			}
			counter++;
		}
		delay(5);
	}

    munmap(fbp, screensize);
    close(fbfd);
    printf("Done. Thank you. \n");

	system("clear");
	system("./bin/nub");
    return 0;
}
