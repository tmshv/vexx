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

SViewport::SViewport(GCViewport *viewpoint)
  {
  _timer = new QTimer;
  //connect(_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
  //_timer->start( 40 );

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  setViewport(viewpoint);
  }

SViewport::~SViewport()
  {
  }

void SViewport::setViewport(GCViewport *vp)
  {
  _viewport = vp;
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
    GCViewport *vp = viewport();
    vp->x = 0;
    vp->y = 0;
    vp->width = w;
    vp->height = h;
    }
  }

void SViewport::paintGL()
  {
  _renderer.clear();
  const GCViewport *vp = viewport();
  if(!vp)
    {
    qWarning() << "No viewport";
    return;
    }

  const GCRenderable* renderable = vp->source();
  if(!renderable)
    {
    qWarning() << "No renderable";
    return;
    }
  renderable->render(&_renderer);
  }
