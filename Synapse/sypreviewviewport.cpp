#include "sypreviewviewport.h"
#include "sproperty.h"
#include "syimagetexture.h"
#include "3D/GCCamera.h"
#include "3D/GCScene.h"
#include "3D/Renderable/GCPlane.h"

S_IMPLEMENT_PROPERTY(SyPreviewViewport)

SPropertyInformation *SyPreviewViewport::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SyPreviewViewport>("SyPreviewViewport");

  return info;
  }

SyPreviewViewport::SyPreviewViewport()
  {
  }

void SyPreviewViewport::setupViewport(const SyImageInput *input, XAbstractCanvasController **controller)
  {
  GCPerspectiveCamera* cam = addChild<GCPerspectiveCamera>("Camera");
  x.connect(&cam->viewportX);
  y.connect(&cam->viewportY);
  width.connect(&cam->viewportWidth);
  height.connect(&cam->viewportHeight);

  cam->setPosition(XVector3D(0.0f, 0.0f, 10.0f));
  cam->setFocalPoint(XVector3D(0.0f, 0.0f, 0.0f));

  GCManipulatableScene* msc = addChild<GCManipulatableScene>("Scene");
  cam->projection.connect(&msc->cameraProjection);
  cam->viewTransform.connect(&msc->cameraTransform);
  cam->connect(&msc->activeCamera);

  GCShadingGroup *group = msc->addChild<GCShadingGroup>("Groups");
  msc->shadingGroups.addPointer(group);

  if(controller)
    {
    *controller = msc;
    }

  const SPropertyInformation *flatSurfaceInfo = STypeRegistry::findType("FlatSurface");
  SProperty *shader = msc->addChild(flatSurfaceInfo, "Shader");
  GCShader *shaderS = shader->uncheckedCastTo<GCShader>();
  group->shader.setPointed(shaderS);

  SyImageTexture *t = msc->addChild<SyImageTexture>("Texture");
  shaderS->findChild("texture")->uncheckedCastTo<GCTexturePointer>()->setPointed(&t->texture);


  XTransform tr = XTransform::Identity();
  tr.translation() = XVector3D(-1.0f, 0.0f, 0.0f);

  GCGeometryTransform *transform2 = msc->addChild<GCGeometryTransform>("Transform");
  group->geometry.addPointer(transform2);
  transform2->transform = tr;
  GCPlane *plane = msc->addChild<GCPlane>("Plane");
  transform2->geometry.setPointed(&plane->geometry);
  t->width.connect(&plane->width);
  t->height.connect(&plane->height);

  source.setPointed(msc);
  }
