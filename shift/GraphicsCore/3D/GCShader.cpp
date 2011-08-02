#include "GCShader.h"
#include "XRenderer.h"

S_IMPLEMENT_PROPERTY(GCShader)

SPropertyInformation *GCShader::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCShader>("GCShader");

  return info;
  }

GCShader::GCShader()
  {
  }

void GCShader::bind(XRenderer *r) const
  {
  r->setShader(&shader);
  }
