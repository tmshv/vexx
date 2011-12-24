#ifndef SAVIEWPORT_H
#define SAVIEWPORT_H

#include "saglobal.h"
#include "sobserver.h"
#include "sentityweakpointer.h"
#include "X3DCanvas.h"
#include "XGLRenderer.h"

class XRenderer;
class GCScreenRenderTarget;

class SHIFTALTER_EXPORT SViewport : public X3DCanvas, STreeObserver
  {
public:
  SViewport(SEntity *viewpoint);
  ~SViewport();

  SEntity *scene() { return _scene.entity(); }

protected:
  void initializeGL();
  void resizeGL( int w, int h );
  void paintGL();

  virtual void onTreeChange(const SChange *);

  XGLRenderer _renderer;
  QTimer *_timer;

  SEntityWeakPointer _scene;
  QList<GCScreenRenderTarget*> _screenRenderers;
  SEntityWeakPointer _viewport;
  };

#endif // SAVIEWPORT_H
