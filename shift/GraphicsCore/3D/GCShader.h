#ifndef GCSHADER_H
#define GCSHADER_H

#include "sentity.h"
#include "sbaseproperties.h"

class GCShader : public SEntity
  {
  S_ENTITY(GCShader, SEntity, 0)
public:
  GCShader();
  };

S_TYPED_POINTER_TYPE(GCShaderPointer, GCShader);

#endif // GCSHADER_H
