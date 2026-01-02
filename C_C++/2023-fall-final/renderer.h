//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#ifndef RENDERER_H
#define RENDERER_H

#include "plot.h"
#include "image.h"
#include "fill.h"

class Renderer {
private:
  // it's convenient to have these as fields, to avoid the need
  // to pass them explicitly to helper functions
  Plot &m_plot;
  Image *m_img;

  // value semantics prohibited
  Renderer(const Renderer &);
  Renderer &operator=(const Renderer &);

public:
  Renderer(Plot &plot);
  ~Renderer();

  Image *render();

private:
};

#endif // RENDERER_H
