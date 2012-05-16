#include "GCShadingGroup.h"
#include "spropertyinformationhelpers.h"
#include "XGeometry.h"
#include "XModeller.h"
#include "XRenderer.h"
#include "3D/Manipulators/GCButtonManipulator.h"
#include "sprocessmanager.h"

S_IMPLEMENT_TYPED_POINTER_TYPE(GCShadingGroupPointer)
S_IMPLEMENT_TYPED_POINTER_ARRAY_TYPE(GCShadingGroupPointerArray)

S_IMPLEMENT_PROPERTY(GCShadingGroup)

void GCShadingGroup::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&GCShadingGroup::shader, "shader");
    }

  if(data.registerInterfaces)
    {
    info->addInheritedInterface<GCShadingGroup, GCManipulatable>();
    }
  }

GCShadingGroup::GCShadingGroup()
  {
  }

void GCShadingGroup::render(XRenderer *r) const
  {
  xAssert(SProcessManager::isMainThread());

  const GCShader *s = shader.pointed();
  if(s)
    {
    s->bind(r);
    }

  GCRenderArray::render(r);
  }

void GCShadingGroup::addManipulators(SPropertyArray *a, const GCTransform *tr)
  {/*
  xAssert(tr == 0);
  //a->add<GCButtonManipulator>();

  // todo, probably dont do this...!
  for(GCGeometryTransformPointer* geoPtr = geometry.firstChild<GCGeometryTransformPointer>(); geoPtr; geoPtr = geoPtr->nextSibling<GCGeometryTransformPointer>())
    {
    GCGeometryTransform* geo = geoPtr->pointed();
    if(geo)
      {
      geo->addManipulators(a);
      }
    }*/
  }
