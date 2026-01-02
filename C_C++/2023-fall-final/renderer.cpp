//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#include <iostream>
#include <cmath>
#include <cassert>
#include <memory>
#include <stdexcept>
#include "exception.h"
#include "renderer.h"
#include "image.h"
#include "plot.h"

//#define DEBUG_FILL
//#define DEBUG_PLOT

using std::cerr; 
using std::cout; 
using std::endl; 

Renderer::Renderer(Plot &plot)
  : m_plot(plot)
  , m_img(nullptr) {
}

Renderer::~Renderer() {
  // Note: the Renderer object does not "own" the Image object,
  // since the render() member function transfers ownership of
  // the Image object to the caller (in the main function)
  //delete[] m_img;
}

Image *Renderer::render() {

  int width = m_plot.get_width();
  int height = m_plot.get_height();

  // the unique_ptr will take care of deleting the Image object
  // if an exception is thrown
  std::unique_ptr<Image> img(new Image(width, height));
  m_img = img.get();

  //Tries to generate the y value array and catches any exception that is thrown
  for(int i = 0; i < (int)m_plot.funcs.size(); i++){
    try{
      m_plot.funcs[i]->generateYarr(m_plot.bounds->get_xmin(), m_plot.bounds->get_xmax(), m_plot.bounds->get_width());  
    }
    catch(std::runtime_error &e) {
      cerr << "Error: " << e.what() << endl; //Error message
      return nullptr; 
    }
  }
  
  //convert all row y values into pixel points for image generation 
  m_plot.generateArrPixelPoints();

  //Fills image with the corresponding order that was read in file
  for(int i = 0; i < (int)m_plot.fillVec.size(); i++){
    //Calls function corresponding to type of fill
    if(m_plot.fillVec[i]->isAbove()){
      m_img->fillAbove(m_plot.arrPixels[m_plot.fillVec[i]->getIndex1()], m_plot.fillVec[i]->getFillColor(), m_plot.fillVec[i]->getOpacity());
    } 
    else if(m_plot.fillVec[i]->isBelow()){
      m_img->fillBelow(m_plot.arrPixels[m_plot.fillVec[i]->getIndex1()], m_plot.fillVec[i]->getFillColor(), m_plot.fillVec[i]->getOpacity());
    }
    else{
      m_img->fillBetween(m_plot.arrPixels[m_plot.fillVec[i]->getIndex1()], m_plot.arrPixels[m_plot.fillVec[i]->getIndex2()], m_plot.fillVec[i]->getFillColor(), m_plot.fillVec[i]->getOpacity());
    }
  }

  //After finishes all fills, changes pixels where the function is to specified color
  for(int i = 0; i < (int)m_plot.arrPixels.size(); i++){
    m_img->setPixels(m_plot.arrPixels[i], m_plot.funcs[i]->getColor());
  }

 return img.release();
}
