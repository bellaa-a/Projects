//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#include <cmath>
#include "exception.h"
#include "expr.h"

//Default constructor, setting all properties of false 
Expr::Expr() {
  isSin = false;
  isCos = false; 
  isPlus = false; 
  isMinus = false; 
  isMul = false; 
  isDiv = false;
  isPi = false; 
  isNum = false; 
  isX = false;
  isInvalid = false; 
  num = 0.00;
}

Expr::~Expr() {
}