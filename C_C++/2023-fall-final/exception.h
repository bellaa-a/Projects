//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>
#include <string>

class PlotException : public std::runtime_error {
public:
  PlotException(const std::string &msg);
  PlotException(const PlotException &other);
  ~PlotException();
};

#endif // EXCEPTION_H
