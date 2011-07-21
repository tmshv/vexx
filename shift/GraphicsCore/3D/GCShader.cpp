#include "GCShader.h"

S_IMPLEMENT_PROPERTY(GCShader)

SPropertyInformation *GCShader::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCShader>("GCShader");

  return info;
  }

GCShader::GCShader()
  {
  }
