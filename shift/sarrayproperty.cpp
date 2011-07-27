#include "sarrayproperty.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(SFloatArrayProperty)

SPropertyInformation *SFloatArrayProperty::createTypeInformation()
  {
  return SPropertyInformation::create<SFloatArrayProperty>("SFloatArrayProperty");
  }

S_IMPLEMENT_PROPERTY(SUIntArrayProperty)

SPropertyInformation *SUIntArrayProperty::createTypeInformation()
  {
  return SPropertyInformation::create<SUIntArrayProperty>("SUIntArrayProperty");
  }


S_IMPLEMENT_PROPERTY(SVector3ArrayProperty)

SPropertyInformation *SVector3ArrayProperty::createTypeInformation()
  {
  return SPropertyInformation::create<SVector3ArrayProperty>("SVector3ArrayProperty");
  }
