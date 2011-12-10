#include "GCStandardSurface.h"
#include "QFile"

void GCStandardSurface::postCreate(GCStandardSurface *surface)
  {
    {
    QFile shaderResource(":/GLResources/shaders/standardSurface.frag");
    if(shaderResource.open(QIODevice::ReadOnly))
      {
      surface->fragment.assign(shaderResource.readAll());
      }
    }

    {
    QFile shaderResource(":/GLResources/shaders/standardSurface.vert");
    if(shaderResource.open(QIODevice::ReadOnly))
      {
      surface->vertex.assign(shaderResource.readAll());
      }
    }

  surface->components.addPointer(&surface->fragment);
  surface->components.addPointer(&surface->vertex);
  }

S_IMPLEMENT_PROPERTY(GCStandardSurface);

SPropertyInformation *GCStandardSurface::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCStandardSurface>("GCStandardSurface", postCreate);

  SPropertyInstanceInformation *rtShader = info->child(&GCStandardSurface::runtimeShader);

  ColourProperty::InstanceInformation *diffuse = info->add(&GCStandardSurface::diffuse, "diffuse");
  diffuse->setDefault(XColour(0.6f, 0.6f, 0.6f, 1.0f));
  diffuse->setAffects(rtShader);

  info->add(&GCStandardSurface::vertex, "vertex");
  info->add(&GCStandardSurface::fragment, "fragment");

  return info;
  }

GCStandardSurface::GCStandardSurface()
  {
  }
