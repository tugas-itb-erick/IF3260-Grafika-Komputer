#pragma once

#include <linux/fb.h>
#include <map>
#include "Color.h"
#include "Drawable.h"
#include "Point.h"
#include "Line.h"
#include "Triangle.h"

class Buffer {
public:
  // Constructors
  Buffer();
  Buffer(int, int);
  Buffer(const Buffer&) = delete; // disable copy constructor

  // Destructor to close framebuffer
  ~Buffer();

  // Getter
  int getWidth();
  int getHeight();
  Drawable& getShape(const string&);
  map<string, Drawable>& getShapes();

  // Methods
  void addLayer(const string&, const vector<Drawable>&);
  void addShape(const string&, const Drawable&);
  void delShape(const string&);
  void clip(vector<Point>& v, const Point& p1, const Point& p2);
  void drawClippedShape(const string& id, int x, int y, const string& clip, int ofx, int ofy, double scale, int posx, int posy, Color cl);
  void drawClippedLayer(const string& id, int x, int y, const string& clip, int ofx, int ofy, double scale, int posx, int posy, Color cl);
  void drawShape(const string&, int, int, Color cl = Color::WHITE);
  void drawShapeBorder(const string&, int, int, Color cl = Color::WHITE);
  void drawScaleShape(const string&, int, int, Color cl = Color::WHITE, double scale = 1, int a = 0, int b = 0);
  void drawScaleShapeBorder(const string&, int, int, Color cl = Color::WHITE, double scale = 1, int a = 0, int b = 0);
  void drawLayer(const string& id, int x, int y, Color cl);
  
  // void scanLineShape(const string& id, double gradient, Color* pattern);
  void reset();
  void apply();

  // Transformation Methods
  void translateShape(const string&, int tx, int ty);
  void translateShape(const string&, const Point&);
  void translateAllShape(const Point&);
  void scaleShape(const string&, double k, int a = 0, int b = 0);
  void scaleShape(const string&, double k, const Point&);
  void scaleAllShape(double k, int a = 0, int b = 0);
  void rotateShape(const string&, double theta, int,int);
  void rotateShape(const string&, double theta, const Point&);
  void rotateShape(const string&, double theta);
  void rotateAllShape(double theta, int a = 0, int b = 0);
  void centerShape(const string&);
  void centerAllShape();
  void setToOrigin(const string&);
  
  // Helper class for overloading [][]
  class Proxy {
  public:
    Proxy(Color* _arr) : _arr(_arr) {}
    Color& operator[](int index) {
      return _arr[index];
    }
  private:
    Color* _arr;
  };

  // Operator overloading [][] to access Color array
  // Kalo mau mapping supaya (0,0) ditengah, urusnya disini
  Proxy operator[](int index) {
    return Proxy(arr[index]);
  }

  static const Point CENTER;

private:
  // Private Methods
  void initFramebuffer();
  void drawPoint(int, int, Color cl = Color::WHITE);
  void drawPoint(const Point&, Color cl = Color::WHITE);
  void drawLine(const Line&, Color cl = Color::WHITE);

  // Attributes
  int width;
  int height;
  Color** arr;
  map<string, Drawable> shapes;
  map<string, vector<Drawable>> layer;

  // Framebuffer Attributes
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  char *fbp = 0;
  int fbfd;
  long int screensize;
};
