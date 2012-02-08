#include "viewport.h"
#include "QApplication"
#include "QTimer"
#include "QAction"
#include "QKeyEvent"
#include "QFile"
#include "QMouseEvent"
#include "QVBoxLayout"
#include "XGLRenderer.h"
#include "XScene.h"
#include "UI/sentityui.h"
#include "3D/GCCamera.h"
#include "3D/GCViewport.h"
#include "3D/GCScene.h"
#include "3D/Shaders/GCStandardSurface.h"
#include "3D/GCShadingGroup.h"
#include "3D/GCScreenRenderTarget.h"
#include "3D/Renderable/GCCuboid.h"
#include "3D/GCTexture.h"
#include "3D/Renderable/GCPlane.h"
#include "object.h"

Viewport::Viewport(SPlugin &db) : SViewport(db.db().addChild<GCViewport>("SomeScene")), UISurface("Viewport", this, UISurface::Dock)
  {
  GCViewport *vp = viewport();

  GCPerspectiveCamera* cam = vp->addChild<GCPerspectiveCamera>("Camera");
  vp->x.connect(&cam->viewportX);
  vp->y.connect(&cam->viewportY);
  vp->width.connect(&cam->viewportWidth);
  vp->height.connect(&cam->viewportHeight);

  cam->setPosition(XVector3D(0.0f, 0.0f, 10.0f));
  cam->setFocalPoint(XVector3D(0.0f, 0.0f, 0.0f));

  GCManipulatableScene* msc = vp->addChild<GCManipulatableScene>("Scene");
  cam->projection.connect(&msc->cameraProjection);
  cam->viewTransform.connect(&msc->cameraTransform);
  cam->connect(&msc->activeCamera);
  setController(msc);

  GCShadingGroup *group = msc->addChild<GCShadingGroup>("Groups");
  GCShadingGroup *group2 = msc->addChild<GCShadingGroup>("Groups");
  msc->shadingGroups.addPointer(group);
  msc->shadingGroups.addPointer(group2);

  const SPropertyInformation *standardSurfaceInfo = STypeRegistry::findType("StandardSurface");
  SProperty *shader = msc->addChild(standardSurfaceInfo, "Shader");
  group->shader.setPointed(shader->uncheckedCastTo<GCShader>());

  const SPropertyInformation *flatSurfaceInfo = STypeRegistry::findType("FlatSurface");
  SProperty *shader2 = msc->addChild(flatSurfaceInfo, "Shader");
  GCShader *shaderS = shader2->uncheckedCastTo<GCShader>();
  group2->shader.setPointed(shaderS);

  GCTexture *t = msc->addChild<GCTexture>("Texture");
  shaderS->findChild("texture")->uncheckedCastTo<GCTexturePointer>()->setPointed(t);


  XTransform tr = XTransform::Identity();
  tr.translation() = XVector3D(1.0f, 0.0f, 0.0f);

  GCGeometryTransform *transform = msc->addChild<GCGeometryTransform>("Transform");
  group->geometry.addPointer(transform);
  transform->transform = tr;


  tr.translation() = XVector3D(-1.0f, 0.0f, 0.0f);

  GCGeometryTransform *transform2 = msc->addChild<GCGeometryTransform>("Transform");
  group2->geometry.addPointer(transform2);
  transform2->transform = tr;

  GCCuboid *cube = msc->addChild<GCCuboid>("Cube");
  transform->geometry.setPointed(&cube->geometry);

  GCPlane *plane = msc->addChild<GCPlane>("Plane");
  transform2->geometry.setPointed(&plane->geometry);

  vp->source.setPointed(msc);
  }

Viewport::~Viewport()
  {
  }

void Viewport::setObject(Object *)
  {
  }
