#include "sadocument.h"

S_IMPLEMENT_PROPERTY(SDocument)

SPropertyInformation *SDocument::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SDocument>("SDocument");

  info->add(&SDocument::transientData, "transientData");

  return info;
  }

SDocument::SDocument()
  {
  }
