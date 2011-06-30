#include "3D/GCTransform.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(GCTransform)

const SPropertyInformation *GCTransform::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCTransform>("GCTransform");
  return info;
  }

GCTransform::GCTransform()
{
}
