//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#ifndef FN_PARSER_H
#define FN_PARSER_H

#include <iostream>
#include <vector>
#include <string>
#include "expr.h"

using std::vector; 
using std::string; 

class ExprParser {
private:
  void insertNode(char letter, char LeftRightHead); 
  ExprParser(const ExprParser &);
  ExprParser &operator=(const ExprParser &);

public:
  Expr * root; 
  vector<string> strVec; 
  int curIndx; 
  ExprParser();
  ~ExprParser();
  Expr * parse();
  void tostring(Expr *cur);
  void print(Expr *cur);
  double calculate(Expr *cur, double x);
  Expr * clear(Expr * cur);
  void checkError(Expr * root);
  void storeStrVec(std::istream &in); 
  void printStrVec(); 
  void checkBracket(); 
};

bool isDouble(std::string myString);
#endif // FN_PARSER_H