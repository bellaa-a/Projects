//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#include "exception.h"

PlotException::PlotException(const std::string &msg)
  : std::runtime_error(msg) {
    
}

PlotException::PlotException(const PlotException &other)
  : std::runtime_error(other) {
}

PlotException::~PlotException() {
}
