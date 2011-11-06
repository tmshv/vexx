#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "sentityweakpointer.h"
#include "UISurface.h"
#include "XObject"
#include "saviewport.h"
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

class Viewport : public SViewport, public UISurface
  {
  Q_OBJECT

public:
  Viewport(SPlugin &);
  ~Viewport();

public slots:
  void setObject(Object *id);
  };

#endif // VIEWPORT_H
