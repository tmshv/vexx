#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "UISurface.h"
#include "XObject"
#include "QGLWidget"
#include "XCamera.h"
#include "XScene.h"
#include "XEnvironmentViewer.h"

class XEnvironment;
class XEnvironmentRenderer;
class XGLRenderer;

class Interface;
class EnvironmentEntity;

class Viewport : public QGLWidget, public UISurface, XEnvironmentViewer
  {
  Q_OBJECT
public:
  Viewport();

  void initializeGL();
  void resizeGL( int w, int h );
  void paintGL();

  void keyPressEvent( QKeyEvent *event );
  void keyReleaseEvent( QKeyEvent *event );
  void mouseMoveEvent( QMouseEvent *event );
  void mousePressEvent( QMouseEvent *event );
  void showEvent(QShowEvent *);

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
  XEnvironment *_env;
  XEnvironmentRenderer *_envRenderer;
  Interface *_interface;
  //EnvironmentEntity *_entity;
  QWidget *_properties;
  };

#endif // VIEWPORT_H
