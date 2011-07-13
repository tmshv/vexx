#include "viewport.h"
#include "QApplication"
#include "QTimer"
#include "QAction"
#include "QKeyEvent"
#include "QMouseEvent"
#include "QVBoxLayout"
#include "XGLRenderer.h"
#include "XScene.h"
#include "Interface.h"
#include "NewItemDialog.h"
#include "EnvironmentEntity.h"
#include "XEnvironmentRenderer.h"
#include "sentityui.h"
#include "application.h"
#include "3D/GCRenderToScreen.h"
#include "3D/GCCamera.h"
#include "3D/GCViewport.h"
#include "3D/GCScene.h"

Viewport::Viewport(Application *app, SPlugin &db) : UISurface("Viewport", this, UISurface::Dock),
    _app(app),
    _db(0),
    _controller(this, this)
  {
  _timer = new QTimer;
  connect( _timer, SIGNAL(timeout()), this, SLOT(updateGL()) );
  _timer->start( 40 );

  setMouseTracking( true );


  _db = &db.db();
  _db->addTreeObserver(this);

  GCViewport* vp = _db->addChild<GCViewport>("Viewport");
  _viewport = vp;
  GCRenderToScreen* op = _db->addChild<GCRenderToScreen>("Output");

  GCPerspectiveCamera* cam = _db->addChild<GCPerspectiveCamera>("Camera");
  vp->aspectRatio.connect(&cam->aspectRatio);

  GCScene* scene = _db->addChild<GCScene>("Scene");
  cam->projection.connect(&scene->cameraProjection);

  op->source.setPointed(scene);
  }

Viewport::~Viewport()
  {
  _screenRenderers.clear();
  _db->removeTreeObserver(this);
  }

void Viewport::onTreeChange(const SChange *c)
  {
  const SPropertyContainer::TreeChange* t = c->castTo<SPropertyContainer::TreeChange>();
  if(!t)
    {
    return;
    }

  GCRenderToScreen *prop = t->property()->castTo<GCRenderToScreen>();
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

void Viewport::zoom(float factor, float x, float y)
  {
  qDebug() << "zoom" << factor;
  }

void Viewport::track(float x, float y)
  {
  qDebug() << "track" << x << y;
  }

void Viewport::dolly(float x, float y)
  {
  qDebug() << "dolly" << x << y;
  }

void Viewport::pan(float x, float y)
  {
  qDebug() << "pan" << x << y;
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
    vp->width = w;
    vp->height = h;
    }
  }

void Viewport::paintGL()
  {
  _renderer.clear();
  foreach(GCRenderToScreen *sc, _screenRenderers)
    {
    const GCRenderable* renderable = sc->source();
    if(renderable)
      {
      renderable->render(&_renderer);
      }
    }
  }
