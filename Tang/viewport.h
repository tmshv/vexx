#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "UISurface.h"
#include "XObject"
#include "QGLWidget"
#include "XCamera.h"
#include "XScene.h"
#include "XEnvironmentViewer.h"

class XEnvironmentRenderer;
class XGLRenderer;
class Application;

class EnvironmentEntity;

class Viewport : public QGLWidget, public UISurface, XEnvironmentViewer
  {
  Q_OBJECT
public:
  Viewport(Application *env);

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
  };

#endif // VIEWPORT_H
