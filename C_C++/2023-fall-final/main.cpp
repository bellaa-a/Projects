//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include "exception.h"
#include "plot.h"
#include "reader.h"
#include "renderer.h"

using std::cout; 
using std::endl; 

int main(int argc, char **argv) {
  //Command line should have three arguments
  if (argc != 3) {
    std::cerr << "Usage: plot <input file> <output file>\n";
    return 1;
  }

  //Reads file from command line
  std::ifstream in(argv[1]);
  if (!in.is_open()) {
    std::cerr << "Error: couldn't open input file " << argv[1] << "\n";
    return 1;
  }

  Plot * newPlot = new Plot(); 
  Reader * newReader = new Reader(); 
  //Checks if able to read input from file
  if(newReader->read_input(in, newPlot) == 0){
    in.close(); 
    delete newReader; 
    delete newPlot; 
    return 1; 
  }

  //To create actual image
  Renderer * renderer = new Renderer(*newPlot); 
  std::unique_ptr<Image> img(renderer->render()); 
  //if an error is catched during the redering process,return a nullptr 
  if(img == nullptr){
    in.close(); 
    img.reset();
    delete renderer; 
    delete newReader; 
    delete newPlot; 
    return 1; 
  }

  //Writes image into the output file 
  else {
    std::ofstream out(argv[2]);
    if (!out.is_open()) {
      std::cerr << "Error: couldn't open output file " << argv[2] << "\n";
      return 1;
    }
    img->write_png(out);
    std::cout << "Wrote plot image successfully!\n";
    out.close(); 
    in.close(); 
    img.reset();
    delete renderer; 
    delete newReader; 
    delete newPlot; 
  }
  return 0;
}
