#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "UISurface.h"
#include "XObject"
#include "QGLWidget"
#include "XCamera.h"
#include "XScene.h"
#include "XEnvironmentViewer.h"
#include "sentityweakpointer.h"
#include "splugin.h"

class XEnvironmentRenderer;
class XGLRenderer;
class Application;

class GCRenderToScreen;

class EnvironmentEntity;

class Viewport : public QGLWidget, public UISurface, XEnvironmentViewer, STreeObserver
  {
  Q_OBJECT
public:
  Viewport(Application *env, SPlugin &);
  ~Viewport();

  void initializeGL();
  void resizeGL( int w, int h );
  void paintGL();

  void keyPressEvent( QKeyEvent *event );
  void keyReleaseEvent( QKeyEvent *event );
  void mouseMoveEvent( QMouseEvent *event );
  void mousePressEvent( QMouseEvent *event );
  void showEvent(QShowEvent *);

  X_ALIGNED_OPERATOR_NEW

private slots:
  void newItem();

private:
  QGLContext *initRenderer();

  virtual void onTreeChange(const SChange *);

  virtual XVector3D screenToWorld(const QPoint &pt) const;
  virtual XVector3D position() const;
  virtual xReal farClipPlane() const;
  virtual xReal nearClipPlane() const;
  virtual XFrustum frustum() const;

  XGLRenderer *_renderer;
  XPerspectiveCamera _camera;
  XScene _scene;
  QTimer *_timer;
  QPoint _oldPos;
  bool _initMouse;
  XList <int> _move;
  XEnvironmentRenderer *_envRenderer;
  //EnvironmentEntity *_entity;
  QWidget *_properties;
  Application *_app;

  SAppDatabase *_db;
  QList<GCRenderToScreen*> _screenRenderers;
  SEntityWeakPointer _viewport;
  };

#endif // VIEWPORT_H
