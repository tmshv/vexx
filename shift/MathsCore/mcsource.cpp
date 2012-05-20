#include "mcsource.h"
#include "spropertyinformationhelpers.h"

S_IMPLEMENT_PROPERTY(MCSource, MathsCore)

void MCSource::createTypeInformation(SPropertyInformationTyped<MCSource> *info,
                                     const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&MCSource::output, "output");
    }
  }

MCSource::MCSource()
  {
  }
