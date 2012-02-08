#include "mcsource.h"

S_IMPLEMENT_PROPERTY(MCSource)

SPropertyInformation *MCSource::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<MCSource>("MCSource");

  info->add(&MCSource::output, "output");

  return info;
  }

MCSource::MCSource()
  {
  }
