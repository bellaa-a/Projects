//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#include <cassert>
#include <string>
#include "fill.h"

//Default constructor
Fill::Fill() {
   isFillAbove = false;
   isFillBelow = false;
   isFillBetween = false;
   cnew.r = 0;
   cnew.g = 0;
   cnew.b = 0;
   opacity = 0.0;
   firstIndex = -1;
   secondIndex = -1;
}

//Destructor
Fill::~Fill() {
}

//Sets incoming fill color, initially read from file
void Fill::setNewColor(int r, int g, int b){
   this->cnew.r = r;
   this->cnew.g = g;
   this->cnew.b = b;
}
