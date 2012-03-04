#include "syimageoutput.h"

S_IMPLEMENT_PROPERTY(SyImageOutput)

SPropertyInformation *SyImageOutput::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SyImageOutput>("SyImageOutput");

  //info.

  return info;
  }

SyImageOutput::SyImageOutput()
  {
  }
