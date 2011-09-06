#ifndef STANDARDSURFACE_H
#define STANDARDSURFACE_H

#include "3D/GCShader.h"

class GRAPHICSCORE_EXPORT GCStandardSurface : public GCShader
  {
  S_ENTITY(GCStandardSurface, GCShader, 0)
public:
  GCStandardSurface();

  class Colour : public ColourProperty
    {
    };

  Colour diffuse;

protected:
  GCFragmentShaderComponent fragment;
  GCVertexShaderComponent vertex;

private:
  static void postCreate(GCStandardSurface *);
  };

#endif // STANDARDSURFACE_H
