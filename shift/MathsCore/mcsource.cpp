#include "mcsource.h"

S_IMPLEMENT_PROPERTY(MCSource)

void MCSource::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&MCSource::output, "output");
    }
  }

MCSource::MCSource()
  {
  }
