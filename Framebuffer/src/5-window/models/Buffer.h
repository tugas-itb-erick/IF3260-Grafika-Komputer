#pragma once

#include <linux/fb.h>
#include <unordered_map>
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
  
  void drawLine(const Line&, Color cl = Color::WHITE);
  void drawClippedShape(const string& id, int x, int y, const string& clip, int ofx, int ofy, double scale, Color cl);
  // Getter
  int getWidth();
  int getHeight();
  Drawable& getShape(const string&);
  unordered_map<string, Drawable>& getShapes();

  // Methods
  void addShape(const string&, const Drawable&);
  void delShape(const string&);
  void clip(vector<Point>& v, const Point& p1, const Point& p2);
  void drawShape(const string&, int, int, Color cl = Color::WHITE, bool b = true);
  
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
  static const Point CLIP[];

private:
  // Private Methods
  void initFramebuffer();
  bool drawPoint(int, int, Color cl = Color::WHITE);
  void drawPoint(const Point&, Color cl = Color::WHITE);
  

  // Attributes
  int width;
  int height;
  Color** arr;
  unordered_map<string, Drawable> shapes;

  // Framebuffer Attributes
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  char *fbp = 0;
  int fbfd;
  long int screensize;
};
