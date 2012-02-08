#include "syimageinput.h"

S_IMPLEMENT_PROPERTY(SyImageInput)

SPropertyInformation *SyImageInput::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SyImageInput>("SyImageInput");

  //info.

  return info;
  }

SyImageInput::SyImageInput()
  {
  }
