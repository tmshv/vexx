#ifndef GCGRID_H
#define GCGRID_H

#include "3D/GCRenderable.h"

class GCGrid : public GCRenderable
  {
  S_PROPERTY(GCGrid, GCRenderable, 0)

public:
  GCGrid();

  virtual void render();
  };

#endif // GCGRID_H
