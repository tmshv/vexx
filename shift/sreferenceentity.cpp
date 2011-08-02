#include "sreferenceentity.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(SReferenceEntity)

SPropertyInformation *SReferenceEntity::createTypeInformation()
  {
  return SPropertyInformation::create<SReferenceEntity>("SReferenceEntity");
  }

SReferenceEntity::SReferenceEntity()
  {
  }
