#include "saviewport.h"
#include "3D/GCCamera.h"
#include "3D/GCViewport.h"
#include "3D/GCScene.h"
#include "3D/Shaders/GCStandardSurface.h"
#include "3D/GCShadingGroup.h"
#include "3D/GCScreenRenderTarget.h"
#include "3D/Renderable/GCCuboid.h"
#include "QTimer"
#include "XScene.h"

SViewport::SViewport(SEntity *viewpoint) : _scene(viewpoint)
  {
  _timer = new QTimer;
  connect(_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
  _timer->start( 40 );

  _scene->addTreeObserver(this);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  }

SViewport::~SViewport()
  {
  _screenRenderers.clear();
  if(_scene.isValid())
    {
    _scene->removeTreeObserver(this);
    }
  }

void SViewport::onTreeChange(const SChange *c)
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

  if(t->after() && t->after() == &_scene->children)
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

void SViewport::initializeGL()
  {
  X3DCanvas::initializeGL();
  _renderer.setContext(const_cast<QGLContext*>(context()));
  _renderer.intialise();
  }

void SViewport::resizeGL( int w, int h )
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

void SViewport::paintGL()
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
