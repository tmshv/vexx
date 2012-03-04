#include "GCGeometry.h"
#include "styperegistry.h"
#include "sprocessmanager.h"

S_IMPLEMENT_PROPERTY(GCGeometry)

void GCGeometry::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
  	info->add(&GCGeometry::runtimeGeometry, "runtimeGeometry");
    }
  }

GCGeometry::GCGeometry()
  {
  }
