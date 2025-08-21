//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#ifndef EXPR_H
#define EXPR_H

#include <vector>

// Base expression class
class Expr {
private:
  Expr(const Expr &);
  Expr &operator=(const Expr &);

  public:
  bool isSin; 
  bool isCos; 
  bool isPlus; 
  bool isMinus; 
  bool isMul; 
  bool isDiv; 
  bool isX; 
  bool isNum; 
  bool isPi;
  bool isInvalid; 
  double num; 

  std::vector<Expr *> exprNodes; 
  Expr();
  ~Expr();
};

#endif // EXPR_H