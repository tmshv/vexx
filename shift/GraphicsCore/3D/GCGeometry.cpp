#include "GCGeometry.h"
#include "spropertyinformationhelpers.h"
#include "styperegistry.h"
#include "sprocessmanager.h"

S_IMPLEMENT_PROPERTY(GCGeometry, GraphicsCore)

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

S_IMPLEMENT_TYPED_POINTER_TYPE(GCGeometryPointer, GraphicsCore)
