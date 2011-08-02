#include "GCScene.h"
#include "XShader.h"
#include "XRenderer.h"
#include "XMatrix4x4"
#include "siterator.h"

S_IMPLEMENT_PROPERTY(GCScene)

SPropertyInformation *GCScene::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCScene>("GCScene");

  info->add(&GCScene::cameraTransform, "cameraTransform");
  info->add(&GCScene::cameraProjection, "cameraProjection");

  info->add(&GCScene::shadingGroups, "shadingGroups");

  return info;
  }

GCScene::GCScene()
  {
  }

void GCScene::render(XRenderer *r) const
  {
  r->setProjectionTransform(cameraProjection());

  r->pushTransform(cameraTransform());

  for(const GCShadingGroupPointer* groupPtr = shadingGroups.firstChild<GCShadingGroupPointer>(); groupPtr; groupPtr = groupPtr->nextSibling<GCShadingGroupPointer>())
    {
    const GCShadingGroup* group = groupPtr->pointed();
    if(group)
      {
      group->render(r);
      }
    }

  r->popTransform();
  }
