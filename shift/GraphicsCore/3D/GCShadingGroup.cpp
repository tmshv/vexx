#include "GCShadingGroup.h"
#include "XGeometry.h"
#include "XModeller.h"
#include "XRenderer.h"

S_IMPLEMENT_PROPERTY(GCShadingGroup)

void computeRuntime(const SPropertyInstanceInformation *info, SPropertyContainer *cont)
  {
  GCShadingGroup* rtGeo = cont->uncheckedCastTo<GCShadingGroup>();

  XGeometry x;
  for(const GCGeometryPointer* geoPtr = rtGeo->geometry.firstChild<GCGeometryPointer>(); geoPtr; geoPtr = geoPtr->nextSibling<GCGeometryPointer>())
    {
    const GCGeometry* geo = geoPtr->pointed();
    if(geo)
      {
      geo->appendTo(&x);
      }
    }
  rtGeo->runtimeGeometry = x;
  }

SPropertyInformation *GCShadingGroup::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCShadingGroup>("GCShadingGroup");

  info->add(&GCShadingGroup::shader, "shader");

  GCRuntimeGeometry::InstanceInformation* rtInfo = info->add(&GCShadingGroup::runtimeGeometry, "runtimeGeometry");
  rtInfo->setCompute(computeRuntime);

  GCGeometry::InstanceInformation* geoInfo = info->add(&GCShadingGroup::geometry, "geometry");
  geoInfo->setAffects(rtInfo);

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

  r->drawGeometry(runtimeGeometry());
  }
