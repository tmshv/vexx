#include "GCStandardSurface.h"
#include "QFile"

void GCStandardSurface::postCreate(GCStandardSurface *surface)
  {
    {
    QFile shaderResource(":/GLResources/shaders/default.frag");
    if(shaderResource.open(QIODevice::ReadOnly))
      {
      surface->fragment.source = shaderResource.readAll();
      }
    }

    {
    QFile shaderResource(":/GLResources/shaders/default.vert");
    if(shaderResource.open(QIODevice::ReadOnly))
      {
      surface->vertex.source = shaderResource.readAll();
      }
    }

  surface->components.addPointer(&surface->fragment);
  surface->components.addPointer(&surface->vertex);
  }

S_IMPLEMENT_PROPERTY(GCStandardSurface);

SPropertyInformation *GCStandardSurface::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCStandardSurface>("GCStandardSurface", postCreate);

  info->add(&GCStandardSurface::vertex, "vertex");
  info->add(&GCStandardSurface::fragment, "fragment");

  return info;
  }

GCStandardSurface::GCStandardSurface()
  {
  }
