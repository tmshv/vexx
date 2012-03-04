#ifndef SAVIEWPORT_H
#define SAVIEWPORT_H

#include "saglobal.h"
#include "sobserver.h"
#include "sentityweakpointer.h"
#include "3D/GCViewport.h"
#include "X3DCanvas.h"
#include "XGLRenderer.h"

class XRenderer;
class GCScreenRenderTarget;

class SHIFTALTER_EXPORT SViewport : public X3DCanvas
  {
public:
  SViewport(GCViewport *viewpoint);
  ~SViewport();

  GCViewport *viewport()
    {
    if(!_viewport.isValid())
      {
      return 0;
      }

    SEntity *e = _viewport.entity();
    xAssert(e);
    return e->uncheckedCastTo<GCViewport>();
    }

  void setViewport(GCViewport *vp);

protected:
  void initializeGL();
  void resizeGL( int w, int h );
  void paintGL();

  XGLRenderer _renderer;
  QTimer *_timer;
  SEntityWeakPointer _viewport;
  };

#endif // SAVIEWPORT_H
