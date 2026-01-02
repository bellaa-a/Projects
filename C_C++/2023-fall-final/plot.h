//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#ifndef PLOT_H
#define PLOT_H

#include <vector>
#include <map>
#include <string>
#include "bounds.h"
#include "image.h"
#include "func.h"
#include "fill.h"

class Plot {

private:
  Plot(const Plot &);
  Plot &operator=(const Plot &);

public:
  Plot();
  ~Plot();
  //Vector of ints that stores pixel pointss
  vector<vector<int>> arrPixels; 

  std::vector<Function *> getfuncs() { return funcs; }
  void addfunc(Function * func) { funcs.push_back(func); }
  int get_width() const {return this->bounds->get_width();}
  int get_height() const {return this->bounds->get_height();}
  void generateArrPixelPoints(); 
  void generatePixelMargen(vector<int>& pixels);

  void print();

  Bounds * bounds; 
  //vector of Functions that stores Function objects created after reading file
  std::vector<Function *> funcs;
  //Vector of Fill objects that stores information about each fill command in file, in the order they are presented in the file
  vector<Fill*> fillVec; 

};

#endif // PLOT_H