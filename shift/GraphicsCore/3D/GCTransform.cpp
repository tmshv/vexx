#include "3D/GCTransform.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(GCTransform)

SPropertyInformation *GCTransform::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCTransform>("GCTransform");
  return info;
  }

GCTransform::GCTransform()
{
}
