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
class GCScene;
class GCViewableTransform;

class Viewport : public SViewport, public UISurface
  {
  Q_OBJECT

XProperties:
  XROProperty(SEntityWeakPointer, scene);

public:
  Viewport(SPlugin &);
  ~Viewport();

  Q_INVOKABLE GCViewport *viewport();

  Q_INVOKABLE void setScene(GCScene *);
  };

#endif // VIEWPORT_H
