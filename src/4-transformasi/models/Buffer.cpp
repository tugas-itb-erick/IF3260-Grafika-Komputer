#include "Buffer.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <iostream>
#include <queue>
#include <cmath>
using namespace std;

const Point Buffer::CENTER = Point(680, 350);
const Point dp[] = {Point(0, 1), Point(-1, 0), Point(0, -1), Point(1, 0)};

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

void Buffer::drawShape(const string& id, int x, int y, Color cl) {
  for (int i=0; i<shapes[id].triangles.size(); i++) {
    if (canDrawTriangle(shapes[id].triangles[i] + Point(x, y))) {
      drawTriangle(shapes[id].triangles[i] + Point(x, y), cl);
      fillTriangle(shapes[id].triangles[i] + Point(x, y), cl);
    }
  }
}

int Buffer::getWidth() {
  return width;
}

int Buffer::getHeight() {
  return height;
}

Drawable& Buffer::getShape(const string& id) {
  return shapes[id];
}

unordered_map<string, Drawable>& Buffer::getShapes() {
  return shapes;
}

void Buffer::addShape(const string& id, const Drawable& dr) {
  shapes[id] = dr;
}

void Buffer::delShape(const string& id) {
 shapes.erase(id);
}

void Buffer::translateShape(const string& id, int tx, int ty) {
  translateShape(id, Point(tx, ty));
}

void Buffer::translateShape(const string& id, const Point& p) {
  for (auto& e : shapes[id].points) {
    e += p;
  }
  for (auto& e : shapes[id].triangles) {
    e.first += p;
    e.second += p;
    e.third += p;
  }
}

void Buffer::translateAllShape(const Point& p) {
  for (auto shape : shapes) {
    translateShape(shape.first, p);
  }
}

void Buffer::scaleShape(const string& id, double k, int a, int b) {
  for (auto& e : shapes[id].points) {
    e = Point(k*(e.x - a) + a, k*(e.y - b) + b);
  }
  for (auto& e : shapes[id].triangles) {
    e.first = Point(k*(e.first.x - a) + a, k*(e.first.y - b) + b);
    e.second = Point(k*(e.second.x - a) + a, k*(e.second.y - b) + b);
    e.third = Point(k*(e.third.x - a) + a, k*(e.third.y - b) + b);
  }
}

void Buffer::scaleAllShape(double k, int a, int b) {
  for (auto shape : shapes) {
    scaleShape(shape.first, k, a, b);
  }
}

void Buffer::rotateShape(const string& id, double theta, int a, int b) {
  setToOrigin(id);
  int xn, yn;
  for (auto& e:shapes[id].points) {
    xn = e.x*cos(theta) - e.y*sin(theta) + a;
    yn = e.x*sin(theta) + e.y*cos(theta) + b;
    e = Point(xn, yn);
  }
  for (auto& e:shapes[id].triangles) {
    xn = (e.first.x - a)*cos(theta) - (e.first.y - b)*sin(theta) + a;
    yn = (e.first.x - a)*sin(theta) + (e.first.y - b)*cos(theta) + b;
    e.first = Point(xn, yn);
    xn = (e.second.x - a)*cos(theta) - (e.second.y - b)*sin(theta) + a;
    yn = (e.second.x - a)*sin(theta) + (e.second.y - b)*cos(theta) + b;
    e.second = Point(xn, yn);
    xn = (e.third.x - a)*cos(theta) - (e.third.y - b)*sin(theta) + a;
    yn = (e.third.x - a)*sin(theta) + (e.third.y - b)*cos(theta) + b;
    e.third = Point(xn, yn);
  }
  centerShape(id);
}

void Buffer::rotateAllShape(double theta, int a, int b) {
  for (auto shape : shapes) {
    rotateShape(shape.first, theta, a, b);
  }
}

void Buffer::centerShape(const string& id) {
  Point cen = shapes[id].centroid();
  cen.negate();
  cen += CENTER;
  translateShape(id, cen);
}

void Buffer::centerAllShape() {
  for (auto shape : shapes) {
    centerShape(shape.first);
  }
}

void Buffer::setToOrigin(const string& s) {
  Point center = shapes[s].centroid();
  center.negate();
  translateShape(s, center);
}

void Buffer::drawPoint(int x, int y, Color cl) {
	if (x && y && x < width && y < height && arr[x][y] == Color::BLACK) {
		arr[x][y] = cl;
	}
}

void Buffer::drawPoint(const Point& P, Color cl) {
  drawPoint(P.x, P.y, cl);
}

void Buffer::drawLine(const Line& L, Color cl) {
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
      drawPoint(x, y, cl);
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
      drawPoint(x, y, cl);
      e += dx*sign;
      if (2*e >= dy) {
        x += sign;
        e -= dy;
      }
    }
  }
}

bool Buffer::canDrawTriangle(const Triangle& t) {
    if (t.first.y < 5 || t.first.y > vinfo.yres - 10) return false;
    if (t.second.y < 5 || t.second.y > vinfo.yres - 10) return false;
    if (t.third.y < 5 || t.third.y > vinfo.yres - 10) return false;
    if (t.first.x < 5 || t.first.x > vinfo.xres - 10) return false;
    if (t.second.x < 5 || t.second.x > vinfo.xres - 10) return false;
    if (t.third.x < 5 || t.third.x > vinfo.xres - 10) return false;
    return true;
}

void Buffer::drawTriangle(const Triangle& t, Color cl) {
  drawLine(Line(t.first, t.second), cl);
  drawLine(Line(t.first, t.third), cl);
  drawLine(Line(t.third, t.second), cl);
}

void Buffer::fillTriangle(const Triangle& T, Color cl) {
  Point center = T.centroid();
  queue<Point> q;
  q.push(center);

  drawPoint(center, cl);

  while (!q.empty()) {
    Point c = q.front();
    q.pop();

    for (int i = 0; i < 4; ++i) {
      Point next = c + dp[i];
      if (arr[next.x][next.y] == Color::BLACK) {
        drawPoint(next, cl);
        q.push(next);
      }
    }
  }
}
