#pragma once

#include <linux/fb.h>
#include <unordered_map>
#include "Color.h"

class Buffer {
public:
  // Constructors
  Buffer();
  Buffer(int, int);

  // Destructor to close framebuffer
  ~Buffer();

  // Getter
  int getWidth();
  int getHeight();

  // Methods
  void reset();
  void apply();
  void draw();
  

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

  // Operator overloading [][]
  Proxy operator[](int index) {
    return Proxy(arr[index]);
  }

private:
  // Private Methods
  void initFramebuffer();

  // Attributes
  int width;
  int height;
  Color** arr;

  // Framebuffer Attributes
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  char *fbp = 0;
  int fbfd;
  long int screensize;
};