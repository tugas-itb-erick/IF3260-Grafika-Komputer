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

bool Buffer::drawShape(const string& id, int x, int y, Color cl) {
  bool res = false;
  for (auto& e:shapes[id].points) {
    e += Point(x, y);
  }
  Line scanline, edge;
  Point intersect;
  vector<Point> intersection;
  for (int j=0;j<height; ++j) {
    intersection.clear();
    scanline = Line(Point(0, j), Point(width, j));
    for (int i=0;i<shapes[id].points.size();++i) {
      if (i+2<shapes[id].points.size()) {
        edge = Line(shapes[id].points[i], shapes[id].points[i+1]);
        intersect = scanline.intersection(edge);
        if (edge.contains(intersect)) {
          intersection.push_back(intersect);
          if (intersect.y >= edge.first.y && intersect.y >= edge.second.y) {
            intersection.push_back(intersect);
          }
        }
      }
    }
    edge = Line(shapes[id].points[0], shapes[id].points[shapes[id].points.size()-2]);
    intersect = scanline.intersection(edge);
    if (edge.contains(intersect)) {
      intersection.push_back(intersect);
      if (intersect.y >= edge.first.y && intersect.y >= edge.second.y) {
        intersection.push_back(intersect);
      }
    }
    for (int k=1;k<intersection.size();k+=2) {
      int start = intersection[k-1].x, end = intersection[k].x;
      if (start > end) {
        int temp = start;
        start = end;
        end = temp;
      }
      if (start < 0) start = 0;
      if (end >= width) end = width;
      for (int i=start;i<=end;++i) {
        res |= drawPoint(i, j, cl);
      }
    }
  }
  for (auto& e:shapes[id].points) {
    e -= Point(x, y);
  }
  return res;
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
}

void Buffer::scaleShape(const string& id, double k, const Point& p) {
  scaleShape(id, k, p.x, p.y);
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
    xn = e.x*cos(theta) - e.y*sin(theta) ;
    yn = e.x*sin(theta) + e.y*cos(theta) ;
    e = Point(xn, yn);
  }
  // centerShape(id);
  for (auto& e:shapes[id].points) {
	e+= Point(a,b);
  }
}

void Buffer::rotateShape(const string& id, double theta, const Point& p) {
  rotateShape(id, theta, p.x, p.y);
}

void Buffer::rotateShape(const string& id, double theta) {
  Point center = shapes[id].centroid();
  rotateShape(id, theta, center);
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

bool Buffer::drawPoint(int x, int y, Color cl) {
	if (x && y && x < width && y < height) {
    if (arr[x][y] == Color::BLACK) {
		  arr[x][y] = cl;
      return false;
    } else if (arr[x][y] == Color::ORANGE) return true;
    else return false;
	}
  return false;
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