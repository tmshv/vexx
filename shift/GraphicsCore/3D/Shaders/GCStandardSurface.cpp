#include "GCStandardSurface.h"
#include "QFile"

void GCStandardSurface::postCreate(GCStandardSurface *surface)
  {
    {
    QFile shaderResource(":/GLResources/shaders/standardSurface.frag");
    if(shaderResource.open(QIODevice::ReadOnly))
      {
      surface->fragment.source = shaderResource.readAll();
      }
    }

    {
    QFile shaderResource(":/GLResources/shaders/standardSurface.vert");
    if(shaderResource.open(QIODevice::ReadOnly))
      {
      surface->vertex.source = shaderResource.readAll();
      }
    }
  }

S_IMPLEMENT_PROPERTY(GCStandardSurface);

SPropertyInformation *GCStandardSurface::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCStandardSurface>("GCStandardSurface", postCreate);

  SPropertyInstanceInformation *rtShader = info->child(&GCStandardSurface::runtimeShader);

  ColourProperty::InstanceInformation *diffuse = info->add(&GCStandardSurface::diffuse, "diffuse");
  diffuse->setDefault(XColour(0.6f, 0.6f, 0.6f, 1.0f));
  diffuse->setAffects(rtShader);

  GCVertexShaderComponent::InstanceInformation *vertInst = info->add(&GCStandardSurface::vertex, "vertex");
  GCFragmentShaderComponent::InstanceInformation *fragInst = info->add(&GCStandardSurface::fragment, "fragment");

  GCShaderComponentPointerArray::InstanceInformation *componentsInst = info->child(&GCStandardSurface::components);
  SPropertyInformation *componentsInfo = info->extendContainedProperty<GCShaderComponentPointerArray>(componentsInst);

  Pointer::InstanceInformation *fragPtrInst = componentsInfo->add<Pointer>("FragmentPtr");
  fragPtrInst->setDefaultInput(fragInst);

  Pointer::InstanceInformation *vertPtrInst = componentsInfo->add<Pointer>("VertexPtr");
  vertPtrInst->setDefaultInput(vertInst);

  return info;
  }

GCStandardSurface::GCStandardSurface()
  {
  }
