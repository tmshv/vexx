#include "area.h"

S_IMPLEMENT_PROPERTY(Area)

void Area::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&Area::shaders, "shaders");
    }
  }

Area::Area()
  {
  }
