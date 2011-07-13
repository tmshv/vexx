#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "UISurface.h"
#include "XObject"
#include "X3DCanvas.h"
#include "XCamera.h"
#include "XScene.h"
#include "XCameraCanvasController.h"
#include "XAbstractRenderModel.h"
#include "XAbstractDelegate.h"
#include "sentityweakpointer.h"
#include "splugin.h"
#include "XGLRenderer.h"

class XEnvironmentRenderer;
class Application;

class GCRenderToScreen;

class EnvironmentEntity;

class Viewport : public X3DCanvas, public UISurface, STreeObserver, XCameraCanvasController::CameraInterface
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

  virtual void zoom(float factor, float x, float y);
  virtual void track(float x, float y);
  virtual void dolly(float x, float y);
  virtual void pan(float x, float y);

  XGLRenderer _renderer;
  QTimer *_timer;
  Application *_app;

  SAppDatabase *_db;
  QList<GCRenderToScreen*> _screenRenderers;
  SEntityWeakPointer _viewport;

  XCameraCanvasController _controller;
  };

#endif // VIEWPORT_H
