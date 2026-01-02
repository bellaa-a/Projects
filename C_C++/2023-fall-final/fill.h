//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#ifndef FILL_H
#define FILL_H

#include<vector>
#include <string>
#include <iostream>
#include "image.h"
#include "func.h"

class Fill {
private:
    bool isFillAbove = false;
    bool isFillBelow = false;
    bool isFillBetween = false;
    Color cnew;
    float opacity = 0;
    int firstIndex = 0;
    int secondIndex = 0;


public:
    Fill();
    ~Fill();
    //Geters and seters for private variables in Fill
    Color & getFillColor() { return cnew; }
    bool isAbove() { return isFillAbove; }
    bool isBelow() { return isFillBelow; }
    bool isBetween() { return isFillBetween; }
    int getIndex1() { return firstIndex; }
    int getIndex2() { return secondIndex; }
    float getOpacity() { return opacity;}
    void setIndex1(int fn1) { firstIndex = fn1; }
    void setIndex2(int fn2) { secondIndex = fn2; }
    void setOpacity(float op) { opacity = op; }
    void setNewColor(int r, int g, int b);
    int getr(){return (int)cnew.r; }
    int getg(){return (int)cnew.g; }
    int getb(){return (int)cnew.b; }
    void setAbove() { isFillAbove = true; }
    void setBelow() { isFillBelow = true; }
    void setBetween() { isFillBetween = true; }


};
#endif 