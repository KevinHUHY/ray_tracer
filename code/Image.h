#ifndef IMAGE_H
#define IMAGE_H

#include <png.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <algorithm>
using namespace std;

struct pixel_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class Image {
public:
  Image(size_t w, size_t h) 
    : width(w), height(h) 
  {
    pixels = new pixel_t[width * height];
      // pixels = (pixel_t*)calloc(sizeof(pixel_t), width*height);
  }
  
  ~Image() 
  {
      // free(pixels);
    delete[] pixels;
  }
  
  int writePNG(const char* path);
  
  void setColor(int x, int y, double r, double g, double b)
  {
    assert(x >= 0 && x < width && y >= 0 && y < height);
  
  // Clamp to [0,1]
    r = min(1.0, max(r, 0.0));
    g = min(1.0, max(g, 0.0));
    b = min(1.0, max(b, 0.0));
  
    pixel_t *p = pixelAt(x, y);
    p->red = (uint8_t)(r*255);
    p->green = (uint8_t)(g*255);
    p->blue = (uint8_t)(b*255);
  }
    
  int getWidth() { return width; }
  int getHeight() { return height; }

private:
  size_t width, height;
  pixel_t *pixels;
  
  pixel_t *pixelAt(int x, int y)
  {
    return pixels + width * y + x;
  }
};

#endif
