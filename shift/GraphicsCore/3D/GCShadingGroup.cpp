#include "GCShadingGroup.h"
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
  XModeller m(&x, 128);

  m.begin(XModeller::Triangles);
    m.vertex( 0.0f, 1.0f, 0.0f);
    m.vertex(-1.0f,-1.0f, 0.0f);
    m.vertex( 1.0f,-1.0f, 0.0f);
  m.end();
  }

void GCShadingGroup::render(XRenderer *r) const
  {
  const GCShader *s = shader.pointed();
  if(s)
    {
    s->bind(r);
    }

  r->drawGeometry(x);
  }
