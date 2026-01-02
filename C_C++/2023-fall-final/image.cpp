//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#include <vector>
#include <algorithm>
#include "pnglite.h"
#include "exception.h"
#include "image.h"
#include "fill.h"

using std::vector; 
using std::cout; 
using std::endl; 

namespace {

struct InitializePnglite {
  InitializePnglite() {
    ::png_init(0, 0);
  }
};

InitializePnglite init_pnglite;

unsigned ostream_png_write_callback(void* input, size_t size, size_t numel, void* user_pointer) {
  std::ostream &out = *(reinterpret_cast<std::ostream *>(user_pointer));
  out.write(reinterpret_cast<const char *>(input), size * numel);
  if (!out.good()) {
    throw PlotException("Error writing output data");
  }
  return numel;
}

} // end anonymous namespace

//constructor
Image::Image(int width, int height)
  : m_width(width)
  , m_height(height)
  , m_pixels(new Color[width * height]) {

//Entire image is initially black
  for (int i=0; i<(width*height); i++){
    m_pixels[i].r = 0;
    m_pixels[i].g = 0;
    m_pixels[i].b = 0;
  }
}

//Destructor
Image::~Image() {
  delete[] m_pixels;

}

//accessing pixels from image 
Color * Image::get_pixels(){
  return m_pixels;
}

//Sets the color of the function based on corresponding pixel points
void Image::setPixels(vector<int> pixelPoints, Color& pixColor){
  int count = 0;    
  for(int i = 0; i < m_height*m_width; i++){
    if(find(pixelPoints.begin(), pixelPoints.end(), i) != pixelPoints.end()){
      this->m_pixels[i].r = pixColor.r; 
      this->m_pixels[i].g = pixColor.g; 
      this->m_pixels[i].b = pixColor.b; 
      count++;
    }
  }
}

//Given function that write the output png
void Image::write_png(std::ostream &out)
{
  // Set up png_t object for output
  png_t png;
  int rc;

  rc = png_open_write(&png, ostream_png_write_callback, static_cast<void*>(&out));
  if (rc != PNG_NO_ERROR) {
    throw PlotException("Error opening PNG output");
  }

  // get pixel data as unsigned char *
  const unsigned char *pixels_raw_const = reinterpret_cast<const unsigned char *>(get_pixels());
  unsigned char *pixels_raw = const_cast<unsigned char *>(pixels_raw_const);

  // Write PNG data
  rc = png_set_data(&png, unsigned(m_width), unsigned(m_height), 8, PNG_TRUECOLOR,
                   pixels_raw);
  if (rc != PNG_NO_ERROR) {
    throw PlotException("Error converting writing image data as PNG");
  }
}

void Image::fillBelow(vector<int> pixelPoints, Color & cnew, float opacity){
  for(int c = 0; c < m_width; c++){
    for(int r = (m_height*m_width) - m_width; r >= pixelPoints[c]; r-=m_width){
      m_pixels[c + r].r = (1 - opacity) * m_pixels[c + r].r + opacity * cnew.r;
      m_pixels[c + r].g = (1 - opacity) * m_pixels[c + r].g + opacity * cnew.g;
      m_pixels[c + r].b = (1 - opacity) * m_pixels[c + r].b + opacity * cnew.b;
    }
        
  }
}

//Sets corresponding pixel points below the function to blended color of previous color and new color
//The pixel points are in a verticle line below the function at the spcified x pixel point
void Image::fillAbove(vector<int> pixelPoints, Color & cnew, float opacity){
  for(int c = 0; c < m_width; c++){
    for(int r = 0; r < pixelPoints[c]; r +=m_width){
      m_pixels[c + r].r = (1 - opacity) * m_pixels[c + r].r + opacity * cnew.r;
      m_pixels[c + r].g = (1 - opacity) * m_pixels[c + r].g + opacity * cnew.g;
      m_pixels[c + r].b = (1 - opacity) * m_pixels[c + r].b + opacity * cnew.b;
    }
  }
}

//Sets corresponding pixel points between two functions to blended color of previous color and new color
void Image::fillBetween(vector<int> pixelPoints1, vector<int> pixelPoints2, Color & cnew, float opacity){
  int greater = 0; 
  int lesser = 0; 

  //For every pixel point
  for(int c = 0; c < m_width; c++){
    //Difference in y pixel points of the two functions
    int difference = pixelPoints1[c] - pixelPoints2[c]; 
  
    //Sets greater to the larger y pixel point, and lesser to the smaller y pixel point
    if(difference > 0){
      greater = pixelPoints1[c]; 
      lesser = pixelPoints2[c]; 
    }
    else if(difference < 0){
      greater = pixelPoints2[c]; 
      lesser = pixelPoints1[c]; 
    }
    //If they have the same y pixel point
    else{
      greater = 0; 
      lesser = 0; 
    }
    //The pixel points are in a verticle line in between the two function at the spcified x pixel point
    for(int r = lesser; r < greater; r +=m_width){
      m_pixels[r].r = (1 - opacity) * m_pixels[r].r + opacity * cnew.r;
      m_pixels[r].g = (1 - opacity) * m_pixels[r].g + opacity * cnew.g;
      m_pixels[r].b = (1 - opacity) * m_pixels[r].b + opacity * cnew.b;
    }
    
  }
}