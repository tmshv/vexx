#include "scdeclarativesurface.h"
#include "sdatabasemodel.h"
#include "QDeclarativeContext"

ScDeclarativeSurface::ScDeclarativeSurface(const QString &name, const QString &s, UISurface::SurfaceType type, SDatabaseModel *model, const QVariantMap &) : UISurface(name, this, type)
  {
  if(model)
    {
    QDeclarativeContext *ctx = rootContext();
    ctx->setContextProperty("db", model);
    ctx->setContextProperty("external", this);
    }

  //setViewport(new QGLWidget);

  setSource(s);
  setResizeMode(QDeclarativeView::SizeRootObjectToView);

  /*QGraphicsObject *root = rootObject();
  foreach(const QString &s, data.keys())
    {
    root->setProperty(s.toLatin1().constData(), data.value(s));
    }*/
  }

