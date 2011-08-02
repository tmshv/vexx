#ifndef GCSHADER_H
#define GCSHADER_H

#include "GCGlobal.h"
#include "sentity.h"
#include "sbasepointerproperties.h"
#include "XShader.h"

class GRAPHICSCORE_EXPORT GCShader : public SEntity
  {
  S_ENTITY(GCShader, SEntity, 0)
public:
  GCShader();

  void bind(XRenderer *r) const;

private:
  XShader shader;
  };

S_TYPED_POINTER_TYPE(GCShaderPointer, GCShader);

#endif // GCSHADER_H
