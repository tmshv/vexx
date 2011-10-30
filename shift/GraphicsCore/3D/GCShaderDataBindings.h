#ifndef GCSHADERDATABINDINGS_H
#define GCSHADERDATABINDINGS_H

#include "GCShader.h"

namespace GCShaderDataBindings
{
class Vector4 : public GCShaderBindableData
  {
public:
  Vector4();
  virtual void bindData(XShader *, const SProperty *) const;
  };
}

#endif // GCSHADERDATABINDINGS_H
