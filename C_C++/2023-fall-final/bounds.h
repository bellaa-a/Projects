//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#ifndef BOUNDS_H
#define BOUNDS_H

class Bounds {
private:
  // TODO: add fields
  double xmin;
  double ymin;
  double xmax;
  double ymax;
  double width;
  double height;

public:
  Bounds(){
    xmin = 0; 
    ymin = 0; 
    xmax = 0; 
    ymax = 0; 
    width = 0; 
    height = 0; 
  }

  void setxmin(double x) { xmin = x; }
  void setymin(double x) { ymin = x; }
  void setxmax(double x) { xmax = x; }
  void setymax(double x) { ymax = x; }
  void setwidth(double x) { width = x; }
  void setheight(double x) { height = x; }

  double get_xmin() {return xmin;}
  double get_ymin() {return ymin;}
  double get_xmax() {return xmax;}
  double get_ymax() {return ymax;}
  int get_width() {return width;}
  int get_height() {return height;}
  // TODO: add member functions
};

#endif // BOUNDS_H
