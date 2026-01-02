//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#include "func.h"
#include "expr_parser.h"
#include "bounds.h"
#include "color.h"
#include <vector>

using std::vector; 
using std::cout; 
using std::endl; 

//constructor
Function::Function(const std::string &name, ExprParser *expr_parser)
  : m_name(name)
  , m_expr(expr_parser) {
    colorAlrSet = false;
    //Function color is white by default
    color.r = 255; 
    color.g = 255; 
    color.b = 255; 
}

//Default constructor
Function::~Function() {
  delete m_expr;
  m_name = "";
  
}

//sets color by the values from file 
void Function::setColor(int r, int g, int b){
  this->color.r = r; 
  this->color.g = g; 
  this->color.b = b; 
}

//Helper function used in debugging
//Prints current color of function
void Function::getColor2(){
  cout << "r = " << (int)color.r << endl; 
  cout << "g = " << (int)color.g << endl; 
  cout << "b = " << (int)color.b << endl; 
}

//Puts calculated y values with the x value of each point into an vector 
void Function::calculateY(double x){
  double result = m_expr->calculate(m_expr->root, x); 
  yVals.push_back(result); 

}

//Generate y values of the function by calling the calculate function with evenly spaced x values
void Function::generateYarr(double xMin, double xMax, int pixelWidth){
  for(double j = 0; j < pixelWidth; j++){ 
      double xj = xMin + j/pixelWidth*(xMax - xMin); 
      this->calculateY(xj); 
      //recording the x values for debugging purposes 
      this->xVals.push_back(xj); 
    }
}

//Helper function used in debugging
//Prints all y values of the function
void Function::printYarr(){
  cout << "print y _______________" << endl;
  for(int i = 0; i < (int)yVals.size(); i++){
    cout << yVals[i] << endl;
  }
}

//Helper function used in debugging
//Prints all x values of the function
void Function::printXarr(){
   cout << "print x _______________" << endl;
  for(int i = 0; i < (int)yVals.size(); i++){
    cout << xVals[i] << endl;
  }
}