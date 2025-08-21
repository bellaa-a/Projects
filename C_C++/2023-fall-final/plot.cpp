//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#include <cassert>
#include "plot.h"
#include <math.h>

using std::cout; 
using std::endl; 

//Default constructor
Plot::Plot(){
  this->bounds = new Bounds(); 
}

//Destructor 
Plot::~Plot(){
  //Delete all functions
  int numFuncs = funcs.size();
  for (int i=0; i<numFuncs; i++){
    delete funcs[i];
  }
  //Delete all fills
  int numFills = fillVec.size();
  for (int i=0; i<numFills; i++){
    delete fillVec[i];
  }
  delete bounds;
}

//Finds pixel points, including the four surrounding the actual plot point, of each function
void Plot::generateArrPixelPoints(){
  for(int numFunc = 0; numFunc < (int)funcs.size(); numFunc++){
    vector<int> pixels; 
    for(int yValsIndx = 0; yValsIndx < (int)funcs[numFunc]->yVals.size(); yValsIndx++){
      //the formula: i = h - 1 - (y-ymin)/(ymax-ymin)*h
      int val = bounds->get_height() - 1 - (int)floor((funcs[numFunc]->yVals[yValsIndx] - bounds->get_ymin())/(bounds->get_ymax() - bounds->get_ymin())*bounds->get_height());
      //val is the number of rows and yValsIndx is the number of colume 
      //this generates the index of one point inside the pixel vector  
      val = val*bounds->get_width() + yValsIndx; 
      pixels.push_back(val); 
    }
    //Generates four neighboring points
    generatePixelMargen(pixels); 
     //Places pixels in the function into arrPixels
    arrPixels.push_back(pixels);
  }
}

//Helper function used in debugging
void Plot::print() {
  for(int i = 0; i < (int)arrPixels[0].size(); i++){
    std::cout << arrPixels[0][i] << std::endl;
  }
}

//Finds the four pixles surrounding the point from the function
void Plot::generatePixelMargen(vector<int>& pixels){
  int length = (int)pixels.size(); 
  for(int i = 0; i < length; i++){

    //Right pixel
    if(pixels[i] > 0 && pixels[i] % bounds->get_width() != 0){
      pixels.push_back(pixels[i] - 1); 
    }

    //Pixel below
    if(pixels[i] >= bounds->get_width()){
      pixels.push_back(pixels[i] - bounds->get_width()); 
    }

    //Left pixel
    if(pixels[i] < bounds->get_width()*bounds->get_height() && pixels[i] % bounds->get_width() != bounds->get_width()-1){
      pixels.push_back(pixels[i] + 1); 
    }

    //Pixel above
    if(bounds->get_width()*bounds->get_height() - pixels[i] > bounds->get_width()){
      pixels.push_back(pixels[i] + bounds->get_width()); 
    }
  }
}