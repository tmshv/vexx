#include "GCShadingGroup.h"

S_IMPLEMENT_PROPERTY(GCShadingGroup)

SPropertyInformation *GCShadingGroup::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCShadingGroup>("GCShadingGroup");

  info->add(&GCShadingGroup::shader, "shader");

  return info;
  }

GCShadingGroup::GCShadingGroup()
  {
  }
