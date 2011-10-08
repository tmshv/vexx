#include "viewport.h"
#include "QApplication"
#include "QTimer"
#include "QAction"
#include "QKeyEvent"
#include "QMouseEvent"
#include "QVBoxLayout"
#include "XGLRenderer.h"
#include "XScene.h"
#include "sentityui.h"
#include "3D/GCCamera.h"
#include "3D/GCViewport.h"
#include "3D/GCScene.h"
#include "3D/GCShader.h"
#include "3D/GCShadingGroup.h"
#include "3D/GCScreenRenderTarget.h"
#include "3D/Renderable/GCCuboid.h"
#include "object.h"

Viewport::Viewport(SPlugin &db) : UISurface("Viewport", this, UISurface::Dock),
    _db(0)
  {
  _timer = new QTimer;
  connect( _timer, SIGNAL(timeout()), this, SLOT(updateGL()) );
  _timer->start( 40 );

  _db = &db.db();
  _db->addTreeObserver(this);

  GCViewport* vp = _db->addChild<GCViewport>("Viewport");
  _viewport = vp;
  GCScreenRenderTarget* op = _db->addChild<GCScreenRenderTarget>("Output");

  GCPerspectiveCamera* cam = _db->addChild<GCPerspectiveCamera>("Camera");
  vp->x.connect(&cam->viewportX);
  vp->y.connect(&cam->viewportY);
  vp->width.connect(&cam->viewportWidth);
  vp->height.connect(&cam->viewportHeight);

  cam->setPosition(XVector3D(0.0f, 0.0f, 10.0f));
  cam->setFocalPoint(XVector3D(0.0f, 0.0f, 0.0f));

  GCManipulatableScene* scene = _db->addChild<GCManipulatableScene>("Scene");
  cam->projection.connect(&scene->cameraProjection);
  cam->viewTransform.connect(&scene->cameraTransform);
  cam->connect(&scene->activeCamera);
  setController(scene);

  GCShadingGroup *group = _db->addChild<GCShadingGroup>("Groups");
  scene->shadingGroups.addPointer(group);

  GCShader *shader = _db->addChild<GCShader>("Shader");
  group->shader.setPointed(shader);

  XTransform tr = XTransform::Identity();
  tr.translation() = XVector3D(1.0f, 0.0f, 0.0f);

  GCGeometryTransform *transform = _db->addChild<GCGeometryTransform>("Transform");
  group->geometry.addPointer(transform);
  transform->transform = tr;


  tr.translation() = XVector3D(-1.0f, 0.0f, 0.0f);

  GCGeometryTransform *transform2 = _db->addChild<GCGeometryTransform>("Transform");
  group->geometry.addPointer(transform2);
  transform2->transform = tr;

  GCCuboid *cube = _db->addChild<GCCuboid>("Cube");
  transform->geometry.setPointed(&cube->geometry);
  transform2->geometry.setPointed(&cube->geometry);

  op->source.setPointed(scene);
  }

Viewport::~Viewport()
  {
  _screenRenderers.clear();
  if(_db.isValid())
    {
    _db->removeTreeObserver(this);
    }
  }

void Viewport::onTreeChange(const SChange *c)
  {
  const SPropertyContainer::TreeChange* t = c->castTo<SPropertyContainer::TreeChange>();
  if(!t)
    {
    return;
    }

  GCScreenRenderTarget *prop = t->property()->castTo<GCScreenRenderTarget>();
  if(!prop)
    {
    return;
    }

  if(t->after())
    {
    if(!_screenRenderers.contains(prop))
      {
      _screenRenderers << prop;
      }
    }
  else
    {
    _screenRenderers.removeAll(prop);
    }
  }

void Viewport::setObject(Object *id)
  {
  }

void Viewport::initializeGL()
  {
  X3DCanvas::initializeGL();
  _renderer.setContext(const_cast<QGLContext*>(context()));
  _renderer.intialise();
  }

void Viewport::resizeGL( int w, int h )
  {
  X3DCanvas::resizeGL(w, h);
  _renderer.setViewportSize(QSize(w,h));

  if(_viewport.isValid())
    {
    GCViewport *vp = _viewport->uncheckedCastTo<GCViewport>();
    vp->x = 0;
    vp->y = 0;
    vp->width = w;
    vp->height = h;
    }
  }

void Viewport::paintGL()
  {
  _renderer.clear();
  foreach(GCScreenRenderTarget *sc, _screenRenderers)
    {
    const GCRenderable* renderable = sc->source();
    if(renderable)
      {
      renderable->render(&_renderer);
      }
    }
  }
