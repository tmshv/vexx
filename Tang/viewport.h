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

class GCScreenRenderTarget;

class EnvironmentEntity;

class Object;

class Viewport : public X3DCanvas, public UISurface, STreeObserver
  {
  Q_OBJECT

public:
  Viewport(SPlugin &);
  ~Viewport();

public slots:
  void setObject(Object *id);

protected:
  void initializeGL();
  void resizeGL( int w, int h );
  void paintGL();

  virtual void onTreeChange(const SChange *);

  XGLRenderer _renderer;
  QTimer *_timer;

  SAppDatabase *db()
    {
    SEntity *ent = _db->entity();
    if(ent)
      {
      return ent->castTo<SAppDatabase>();
      }
    return 0;
    }

  const SAppDatabase *db() const
    {
    const SEntity *ent = _db->entity();
    if(ent)
      {
      return ent->castTo<SAppDatabase>();
      }
    return 0;
    }

  SEntityWeakPointer _db;
  QList<GCScreenRenderTarget*> _screenRenderers;
  SEntityWeakPointer _viewport;
  };

#endif // VIEWPORT_H
