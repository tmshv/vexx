#include "GCViewport.h"
#include "spropertyinformationhelpers.h"

S_IMPLEMENT_PROPERTY(GCViewport, GraphicsCore)

void GCViewport::createTypeInformation(SPropertyInformationTyped<GCViewport> *info,
                                       const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    auto xInst = info->add(&GCViewport::x, "x");
    xInst->setMode(SPropertyInstanceInformation::Output);

    auto yInst = info->add(&GCViewport::y, "y");
    yInst->setMode(SPropertyInstanceInformation::Output);
    }
  }

GCViewport::GCViewport()
  {
  }
