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
#include "object.h"

Viewport::Viewport(SPlugin &db) : SViewport(&db.db()), UISurface("Viewport", this, UISurface::Dock)
  {
  SEntity *sc = scene();

  GCViewport* vp = sc->addChild<GCViewport>("Viewport");
  _viewport = vp;
  GCScreenRenderTarget* op = sc->addChild<GCScreenRenderTarget>("Output");

  GCPerspectiveCamera* cam = sc->addChild<GCPerspectiveCamera>("Camera");
  vp->x.connect(&cam->viewportX);
  vp->y.connect(&cam->viewportY);
  vp->width.connect(&cam->viewportWidth);
  vp->height.connect(&cam->viewportHeight);

  cam->setPosition(XVector3D(0.0f, 0.0f, 10.0f));
  cam->setFocalPoint(XVector3D(0.0f, 0.0f, 0.0f));

  GCManipulatableScene* msc = sc->addChild<GCManipulatableScene>("Scene");
  cam->projection.connect(&msc->cameraProjection);
  cam->viewTransform.connect(&msc->cameraTransform);
  cam->connect(&msc->activeCamera);
  setController(msc);

  GCShadingGroup *group = sc->addChild<GCShadingGroup>("Groups");
  msc->shadingGroups.addPointer(group);

  const SPropertyInformation *standardSurfaceInfo = STypeRegistry::findType("StandardSurface");
  SProperty *shader = sc->addChild(standardSurfaceInfo, "Shader");
  group->shader.setPointed(shader->uncheckedCastTo<GCShader>());

  XTransform tr = XTransform::Identity();
  tr.translation() = XVector3D(1.0f, 0.0f, 0.0f);

  GCGeometryTransform *transform = sc->addChild<GCGeometryTransform>("Transform");
  group->geometry.addPointer(transform);
  transform->transform = tr;


  tr.translation() = XVector3D(-1.0f, 0.0f, 0.0f);

  GCGeometryTransform *transform2 = sc->addChild<GCGeometryTransform>("Transform");
  group->geometry.addPointer(transform2);
  transform2->transform = tr;

  GCCuboid *cube = sc->addChild<GCCuboid>("Cube");
  transform->geometry.setPointed(&cube->geometry);
  transform2->geometry.setPointed(&cube->geometry);

  op->source.setPointed(msc);
  }

Viewport::~Viewport()
  {
  }

void Viewport::setObject(Object *)
  {
  }
