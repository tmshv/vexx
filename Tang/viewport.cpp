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
#include "3D/GCViewport.h"

QGLFormat fmt( bool multi )
    {
    QGLFormat ret;
    ret.setSampleBuffers( multi );
    return ret;
    }

Viewport::Viewport(Application *app, SPlugin &db) : QGLWidget(fmt(false)),
    UISurface("Tang", this, UISurface::Dock),
    _renderer( new XGLRenderer() ),
    _camera( 55, XVector3D( 0, 5, 10 ) ),
    _scene( _renderer, &_camera ), _initMouse( true ),
    _app(app),
    _db(0)
  {
  _timer = new QTimer;
  connect( _timer, SIGNAL(timeout()), this, SLOT(updateGL()) );
  _timer->start( 40 );

  setMouseTracking( true );

  _move << 0 << 0 << 0;

  _db = &db.db();
  _db->addTreeObserver(this);

  _viewport = _db->addChild<GCViewport>("Viewport");
  _db->addChild<GCRenderToScreen>("Output");
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

void Viewport::initializeGL()
  {
  _renderer->setContext(const_cast<QGLContext*>(context()));
  _renderer->intialise();
  }

void Viewport::resizeGL( int w, int h )
  {
  _renderer->setViewportSize(QSize(w,h));

  if(_viewport.isValid())
    {
    GCViewport *vp = _viewport->uncheckedCastTo<GCViewport>();
    vp->width = w;
    vp->height = h;
    }
  }

void Viewport::paintGL()
  {
  _renderer->clear();
  foreach(GCRenderToScreen *sc, _screenRenderers)
    {
    const GCRenderable* renderable = sc->source();
    if(renderable)
      {
      renderable->render(_renderer);
      }
    }
  }

void Viewport::keyPressEvent( QKeyEvent *event )
  {
  switch( event->key() )
    {
    case Qt::Key_Escape: QApplication::quit(); break;
    case Qt::Key_W: _move[2] = -1; break;
    case Qt::Key_S: _move[2] = 1; break;
    case Qt::Key_A: _move[0] = -1; break;
    case Qt::Key_D: _move[0] = 1; break;
    case Qt::Key_Q: _move[1] = -1; break;
    case Qt::Key_E: _move[1] = 1; break;
    }
  }

void Viewport::keyReleaseEvent( QKeyEvent *event )
  {
  switch( event->key() )
    {
    case Qt::Key_W: _move[2] = 0; break;
    case Qt::Key_S: _move[2] = 0; break;
    case Qt::Key_A: _move[0] = 0; break;
    case Qt::Key_D: _move[0] = 0; break;
    case Qt::Key_Q: _move[1] = 0; break;
    case Qt::Key_E: _move[1] = 0; break;
    }
  }

void Viewport::mouseMoveEvent( QMouseEvent *event )
  {
  if((event->modifiers()&Qt::AltModifier) != false)
    {
    QPointF pan( QPointF( QCursor::pos() - _oldPos ) * 0.05 );

    _camera.pan( pan.x(), -pan.y() );
    }
  _oldPos = QCursor::pos();
  }

void Viewport::mousePressEvent( QMouseEvent *event )
  {
  if(event->button() == Qt::LeftButton)
    {
    /*XEnvironmentRenderer::RayCastResults results = _envRenderer->rayCast(event->pos(), true);

    // clear the UI
    foreach(QObject *child, _properties->children())
      {
      if(child->isWidgetType())
        {
        child->deleteLater();
        }
      }

    // try to fill it again
    if(results.hasResults())
      {
      XEnvironmentRenderer::RayCastResult result = results.closestResult(_camera.position());

      //_entity->setItem(result.ID(), result.parentTree().back(), result.index());

      //SEntityUI ui;
      //_properties->layout()->addWidget(ui.createControlWidget(_entity, _properties));
      }*/
    }
  }

void Viewport::showEvent(QShowEvent *event)
  {
  if(!isVisible())
    {
    QApplication::quit();
    }
  }

void Viewport::newItem()
  {
  /*
  NewItemDialog *dialog = new NewItemDialog(_app->environment(), this);
  dialog->show();*/
  }

XVector3D Viewport::screenToWorld(const QPoint &pt) const
  {
  return _camera.screenToWorld(pt);
  }

XVector3D Viewport::position() const
  {
  return _camera.position();
  }

xReal Viewport::farClipPlane() const
  {
  return _camera.farClipPlane();
  }

xReal Viewport::nearClipPlane() const
  {
  return _camera.nearClipPlane();
  }

XFrustum Viewport::frustum() const
  {
  return _camera.frustum();
  }
