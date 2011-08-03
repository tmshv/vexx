#include "GCShadingGroup.h"
#include "XGeometry.h"
#include "XModeller.h"
#include "XRenderer.h"

S_IMPLEMENT_PROPERTY(GCShadingGroup)

SPropertyInformation *GCShadingGroup::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCShadingGroup>("GCShadingGroup");

  info->add(&GCShadingGroup::shader, "shader");

  info->add(&GCShadingGroup::geometry, "geometry");

  return info;
  }

GCShadingGroup::GCShadingGroup()
  {
  }

void GCShadingGroup::render(XRenderer *r) const
  {
  const GCShader *s = shader.pointed();
  if(s)
    {
    s->bind(r);
    }

  for(const GCGeometryTransformPointer* geoPtr = geometry.firstChild<GCGeometryTransformPointer>(); geoPtr; geoPtr = geoPtr->nextSibling<GCGeometryTransformPointer>())
    {
    const GCGeometryTransform* geo = geoPtr->pointed();
    if(geo)
      {
      geo->render(r);
      }
    }
  }
