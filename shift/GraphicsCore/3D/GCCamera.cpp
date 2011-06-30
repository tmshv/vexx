#include "GCCamera.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(GCCamera)

const SPropertyInformation *GCCamera::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCCamera>("GCCamera");
  return info;
  }

GCCamera::GCCamera()
  {
  }
