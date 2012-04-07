#include "MCShape.h"
#include "XRenderer.h"

S_IMPLEMENT_PROPERTY(MCShape)

void MCShape::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&MCShape::geometry, "geometry");
    }
  }

MCShape::MCShape()
  {
  }

void MCShape::render(XRenderer *r) const
  {
  r->drawGeometry(geometry.runtimeGeometry());
  }
