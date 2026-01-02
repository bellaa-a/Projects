//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#ifndef IMAGE_H
#define IMAGE_H

#include <cstdint>
#include <vector>
#include <iostream>
#include "color.h"
#include "fill.h"

using std::vector; 

class Image {
private:
  int m_width, m_height;
  Color * m_pixels;
  Image(const Image &);
  Image &operator=(const Image &);

public:
  Image(int width, int height);
  ~Image();
  int get_width() const { return m_width; }
  int get_height() const { return m_height; }
  void setPixels(vector<int> pixelPoints, Color& color); 
  Color * get_pixels();
  void fillBelow(vector<int> pixelPoints, Color & cnew, float opacity); 
  void fillAbove(vector<int> pixelPoints, Color & cnew, float opacity);
  void fillBetween(vector<int> pixelPoints1, vector<int> pixelPoints2, Color & cnew, float opacity);
  void write_png(std::ostream &out);
};

#endif // IMAGE_H
