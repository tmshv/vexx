#include "area.h"
#include "spropertyinformationhelpers.h"
#include "tangmanager.h"

S_IMPLEMENT_PROPERTY(Area, Tang)

void Area::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&Area::shaders, "shaders");
    info->add(&Area::shadingGroups, "shaderGroups");
    }
  }

Area::Area()
  {
  }
