#include "GCShaderDataBindings.h"

namespace GCShaderDataBindings
{

Vector4::Vector4() : GCShaderBindableData(true)
  {
  }

void Vector4::bindData(XShader *s, const SProperty *p) const
  {
  const ColourProperty *c = p->uncheckedCastTo<ColourProperty>();

  XShaderVariable *v = s->getVariable(p->name());
  xAssert(v);

  v->setValue(c->value());
  }

}
