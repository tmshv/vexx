#include "GCViewport.h"
#include "spropertyinformationhelpers.h"

S_IMPLEMENT_PROPERTY(GCViewport, GraphicsCore)

void GCViewport::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    UnsignedIntProperty::InstanceInformation *xInst = info->add(&GCViewport::x, "x");
    xInst->setMode(SPropertyInstanceInformation::Output);
    UnsignedIntProperty::InstanceInformation *yInst = info->add(&GCViewport::y, "y");
    yInst->setMode(SPropertyInstanceInformation::Output);
    }
  }

GCViewport::GCViewport()
  {
  }
