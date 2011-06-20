#include "sreferenceentity.h"
#include "sdatabase.h"

S_IMPLEMENT_PROPERTY(SReferenceEntity)

const SPropertyInformation *SPropertyContainer::createTypeInformation()
  {
  return SPropertyInformation::create<SReferenceEntity>("SReferenceEntity");
  }

SReferenceEntity::SReferenceEntity()
  {
  }
