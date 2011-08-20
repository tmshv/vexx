#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "sentityweakpointer.h"
#include "UISurface.h"
#include "XObject"
#include "X3DCanvas.h"
#include "XCamera.h"
#include "XScene.h"
#include "XCameraCanvasController.h"
#include "XAbstractRenderModel.h"
#include "XAbstractDelegate.h"
#include "splugin.h"
#include "XGLRenderer.h"

class XEnvironmentRenderer;
class Application;

class GCRenderToScreen;

class EnvironmentEntity;

class Viewport : public X3DCanvas, public UISurface, STreeObserver
  {
  Q_OBJECT

public:
  Viewport(Application *env, SPlugin &);
  ~Viewport();

protected:
  void initializeGL();
  void resizeGL( int w, int h );
  void paintGL();

  virtual void onTreeChange(const SChange *);

  XGLRenderer _renderer;
  QTimer *_timer;
  Application *_app;

  SAppDatabase *_db;
  QList<GCRenderToScreen*> _screenRenderers;
  SEntityWeakPointer _viewport;

  XCameraCanvasController _controller;
  };

#endif // VIEWPORT_H
