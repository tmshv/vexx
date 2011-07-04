#ifndef GCRENDERABLE_H
#define GCRENDERABLE_H

#include "sentity.h"

class GCRenderable : public SEntity
  {
  S_PROPERTY(GCRenderable, SEntity, 0)

public:
  GCRenderable();

  virtual void render() = 0;
  };

#endif // GCRENDERABLE_H
