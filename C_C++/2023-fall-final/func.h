//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#ifndef FUNC_H
#define FUNC_H

#include <string>
#include <vector>
#include "expr_parser.h"
#include "color.h"

using std::vector; 

class Function {
private:
  // a function has a name and an expression computing its value (given x)
  std::string m_name;
  ExprParser * m_expr;
  Color color; 

  Function(const Function &);
  Function &operator=(const Function &);

public:
  vector<double> yVals; 
  vector<double> xVals; 
  Function(const std::string &name, ExprParser * expr_parser);
  ~Function();
  void calculateY(double x); 
  void generateYarr(double xMin, double xMax, int pixelWidth);
  void printYarr(); 
  void printXarr();
  void setColor(int r, int g, int b); 
  void getColor2(); 
  Color & getColor() {return color; }
  bool colorAlrSet;
  std::string get_name() const { return m_name; }
  ExprParser *get_expr() const { return m_expr; }
};

#endif // FUNC_H
