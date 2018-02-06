#include "Buffer.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <iostream>
using namespace std;

void Buffer::initFramebuffer() {
  fbfd = 0;
  screensize = 0;

  // Open the file for reading and writing
  fbfd = open("/dev/fb0", O_RDWR);
  if (fbfd == -1) {
    perror("Error: cannot open framebuffer device");
  }
  printf("The framebuffer device was opened successfully.\n");

  // Get fixed screen information
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
    perror("Error reading fixed information");
  }

  // Get variable screen information
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
    perror("Error reading variable information");
  }

  printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

  // Figure out the size of the screen in bytes
  screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

  // Map the device to memory
  fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
  // if ((int)fbp == -1) {
  //   perror("Error: failed to map framebuffer device to memory");
  // }
  printf("The framebuffer device was mapped to memory successfully.\n");
}

Buffer::Buffer() {
  initFramebuffer();

  const int MAGIC_NUM = 30;
  width = vinfo.xres;
  height = vinfo.yres - MAGIC_NUM;

  arr = new Color*[width];
  for (int i=0; i<width; i++) {
    arr[i] = new Color[height];
  }

}

Buffer::Buffer(int width, int height) {
  initFramebuffer();

  this->width = width;
  this->height = height;
  
  arr = new Color*[width];
  for (int i=0; i<width; i++) {
    arr[i] = new Color[height];
  }

}

Buffer::~Buffer() {
  for (int i=0; i<width; i++) {
    delete[] arr[i];
  }
  delete[] arr;

  munmap(fbp, screensize);
  close(fbfd);
}

void Buffer::reset() {
  for (int i=0; i<width; i++) {
    for (int j=0; j<height; j++) {
      arr[i][j] = Color::BLACK;
    }
  }
}

void Buffer::apply() {
  long int location = 0;
  for (int i=0; i<width; i++) {
    for (int j=0; j<height; j++) {
      location = (i+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (j+vinfo.yoffset) * finfo.line_length;
      *(fbp + location + 0) = arr[i][j].b; // blue
	 		*(fbp + location + 1) = arr[i][j].g; // green
	 		*(fbp + location + 2) = arr[i][j].r; // red
	 		*(fbp + location + 3) = arr[i][j].a; // transparency
    }
  }
}

void Buffer::draw() {
  
}

int Buffer::getWidth() {
  return width;
}

int Buffer::getHeight() {
  return height;
}