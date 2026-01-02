//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#include <iostream>
#include <map>
#include <sstream>
#include <cstdint>
#include <vector>
#include "exception.h"
#include "plot.h"
#include "reader.h"
#include "expr_parser.h"
#include "color.h"
#include "fill.h"
#include "sstream"


using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;

//Default constructor
Reader::Reader() {
}

//Destructor
Reader::~Reader() {
 }

//Helper functions that ensures values stored are legal 
double isDouble2(string myString);
float isFloat(string myString);
int checkInt(string myString);
//Stores entire line of text file into a vector of strings
void storeStrVec2(std::istream &in, vector<string> & strVec); 

int Reader::read_input(std::istream &in, Plot * plot) {
 std::string key;
 try{
   while (in >> key) {
    //__________________________________________________________________
    //Reads plot information into a string, then converting them to specified format and stores 
    //values into plot
     if (key == "Plot") {
       string expressions;
       vector<string> strVec;
       storeStrVec2(in, strVec);
        //check whether the number of arguments inside fulfills the requirement 
        if(strVec.size() != 6) {
         throw PlotException("Invalid number of arguments");
       }

       plot->bounds->setxmin(isDouble2(strVec[0]));
       plot->bounds->setymin(isDouble2(strVec[1]));
       plot->bounds->setxmax(isDouble2(strVec[2]));
       plot->bounds->setymax(isDouble2(strVec[3]));
       plot->bounds->setwidth(checkInt(strVec[4]));
       plot->bounds->setheight(checkInt(strVec[5]));

      //Minimuns cannot be larger than maximums
       if (plot->bounds->get_xmin() >= plot->bounds->get_xmax() ||
       plot->bounds->get_ymin() >= plot->bounds->get_ymax() ) {
         throw PlotException("invalid plot bound");
       }

      //Width and height cannot be negative
       if (plot->bounds->get_width() <= 0 ||
       plot->bounds->get_height() <= 0) {
         throw PlotException("invalid image dimensions");
       }
     }
    //__________________________________________________________________
     else if (key == "Function") {
      //Reads function name and expression and stores accordingly
       std::string name;
       in >> name;
       ExprParser * expr = new ExprParser();
       try{
         expr->storeStrVec(in);
         expr->parse();
         expr->checkError(expr->root);
         expr->checkBracket();
       }
       catch(std::runtime_error &e){
        //catch any errors during parsing
         cerr << "Error: " << e.what() << endl;
         Function * func = new Function(name, expr);
         plot->addfunc(func);
         return 0;
       }
       //Adds newly read function 
       Function * func = new Function(name, expr);
       plot->addfunc(func);
     }

    //__________________________________________________________________
     else if (key == "Color") {
       string name;
       in >> name;
       int index = -1;
       //Finds index in the array of functions that matches the function name given in the file
       for (size_t i = 0; i < plot->getfuncs().size(); i++) {
         if (plot->funcs[i]->get_name() == name) {
           index = i;
         }
       }

      //throw an error if function name provided doesn't exist
       if (index == -1) {
         throw PlotException("color directive referring to a nonexistent function name");
       }

       int r = 0;
       int g = 0;
       int b = 0;

      //Checks if r g b values are valid
       if(!in.eof()){
         string str;
         in >> str;
         r = checkInt(str);
       } else {
         throw std::runtime_error("invalid color arguments");
       }

       if(!in.eof()){
         string str;
         in >> str;
         g = checkInt(str);
       } else {
         throw std::runtime_error("invalid color arguments");
       }

       if(!in.eof()){
         string str;
         in >> str;
         b = checkInt(str);
       } else {
         throw std::runtime_error("invalid color arguments");
       }

      //If the color of the function has not already been set, sets color to corresponding function
       for(int i = 0; i < (int)plot->funcs.size(); i++){
         if(plot->funcs[i]->get_name() == name){
           if (plot->funcs[i]->colorAlrSet == false){
             plot->funcs[i]->setColor(r,g,b);
             plot->funcs[i]->colorAlrSet = true;
           }
           else{
             throw PlotException("cannot have more than one color directive for a function");
           }
         }
       }
     }

     //__________________________________________________________________

     else if (key == "FillBetween") {
       int index1 = -1;
       string fn1;
       in >> fn1;
      //Finds index in the array of functions that matches the function names given in the file
       for (size_t i = 0; i < plot->getfuncs().size(); i++) {
         if (plot->funcs[i]->get_name() == fn1) {
           index1 = i;
         }
       }
       int index2 = -1;
       string fn2;
       in >> fn2;
       for (size_t i = 0; i < plot->getfuncs().size(); i++) {
         if (plot->funcs[i]->get_name() == fn2) {
           index2 = i;
         }
       }
       //throw an error, if either function name doesn't exist
       if (index1 == -1 || index2 == -1) {
         throw PlotException("fill directive referring to a nonexistent function name");
       }

       string expressions;
       vector<string> strVec;
       storeStrVec2(in, strVec);

      if(strVec.size() != 4) {
         throw PlotException("Invalid number of arguments");
       }

      //Creates new fill object and stores accordingly
       Fill * newfill = new Fill();
       newfill->setOpacity(isFloat(strVec[0]));
       newfill->setIndex1(index1);
       newfill->setIndex2(index2);
       newfill->setNewColor(checkInt(strVec[1]), checkInt(strVec[2]), checkInt(strVec[3]));
       newfill->setBetween();
       plot->fillVec.push_back(newfill);
     }

     //__________________________________________________________________

    else if (key == "FillAbove") {
      int index = -1;
      string fn;
      in >> fn;
       //Finds index in the array of functions that matches the function names given in the file
      for (size_t i = 0; i < plot->getfuncs().size(); i++) {
        if (plot->getfuncs()[i]->get_name() == fn) {
          index = i;
        }
      }
      //If function name doesn't exist
      if (index == -1) {
        throw PlotException("fill directive referring to a nonexistent function name");
      }
      //Checks if r g b values are valid
       string expressions;
       vector<string> strVec; 
       storeStrVec2(in, strVec); 
     
      if(strVec.size() != 4) {
         throw PlotException("Invalid number of arguments");
       }

      //Creates new fill object and stores accordingly
       Fill * newfill = new Fill();
       newfill->setOpacity(isFloat(strVec[0]));
       newfill->setIndex1(index);
       newfill->setNewColor(checkInt(strVec[1]), checkInt(strVec[2]), checkInt(strVec[3]));
       newfill->setAbove();
       plot->fillVec.push_back(newfill);
    }
//__________________________________________________________________

    else if (key == "FillBelow") {
      int index;
      string fn;
      in >> fn;
      //Finds index in the array of functions that matches the function names given in the file
      for (size_t i = 0; i < plot->getfuncs().size(); i++) {
        if (plot->getfuncs()[i]->get_name() == fn) {
          index = i;
        }
      }
      //If function name doesn't exist
      if (index == -1) {
        throw PlotException("fill directive referring to a nonexistent function name");
      }
       vector<string> strVec;
       storeStrVec2(in, strVec);

        if(strVec.size() != 4) {
         throw PlotException("Invalid number of arguments");
       }

      //Checks if r g b values are valid
      //Creates new fill object and stores accordingly
       Fill * newfill = new Fill();
       newfill->setOpacity(isFloat(strVec[0]));
       newfill->setIndex1(index);
       newfill->setNewColor(checkInt(strVec[1]), checkInt(strVec[2]), checkInt(strVec[3]));
       newfill->setBelow();
       plot->fillVec.push_back(newfill);
    }

//__________________________________________________________________
    else{
      //if the key matches non of the cases, throw an error 
      throw PlotException("invalid argument");
      return 0;
    }
  }
}

catch(std::runtime_error& e){
  cerr << "Error: " << e.what() << endl;
  return 0;
}
return 1;
}

void storeStrVec2(std::istream &in, vector<string> & strVec){
   string expressions;
   getline(in, expressions);
   stringstream expressions2(expressions);
   string expr;
   while(expressions2 >> expr){
     strVec.push_back(expr);
   }
}

double isDouble2(string myString) {
 std::istringstream iss(myString);
 double d;
 iss >> d;
 if (!iss.fail()) {
   return std::stod(myString);
 } else {
   throw PlotException("wrong number of arguments to a directive, or invalid arguments to a directive");
 }
}


float isFloat(string myString) {
 std::istringstream iss(myString);
 float d;
 iss >> d;
 if (!iss.fail()) {
   return std::stof(myString);
 } else {
   throw PlotException("wrong number of arguments to a directive, or invalid arguments to a directive");
 }
}


int checkInt(string myString) {
 std::istringstream iss(myString);
 int i;
 iss >> i;
 if (!iss.fail()) {
   return std::stoi(myString);
 } else {
   throw PlotException("wrong number of arguments to a directive, or invalid arguments to a directive");
 }
}

