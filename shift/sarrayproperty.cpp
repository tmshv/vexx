#include "sarrayproperty.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(SFloatArrayProperty)

const SPropertyInformation *SFloatArrayProperty::createTypeInformation()
  {
  return SPropertyInformation::create<SFloatArrayProperty>("SFloatArrayProperty");
  }

S_IMPLEMENT_PROPERTY(SUIntArrayProperty)

const SPropertyInformation *SUIntArrayProperty::createTypeInformation()
  {
  return SPropertyInformation::create<SUIntArrayProperty>("SUIntArrayProperty");
  }
