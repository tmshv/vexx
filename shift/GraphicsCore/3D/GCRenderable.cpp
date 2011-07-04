#include "GCRenderable.h"

S_IMPLEMENT_PROPERTY(GCRenderable)

SPropertyInformation *GCRenderable::createTypeInformation()
  {
  return SPropertyInformation::create<GCRenderable>("GCRenderable");
  }

GCRenderable::GCRenderable()
  {
  }
