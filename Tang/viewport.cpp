#include "viewport.h"
#include "QApplication"
#include "QTimer"
#include "QAction"
#include "QKeyEvent"
#include "QFile"
#include "QMouseEvent"
#include "QVBoxLayout"
#include "XGLRenderer.h"
#include "XScene.h"
#include "UI/sentityui.h"
#include "3D/GCCamera.h"
#include "3D/GCViewport.h"
#include "3D/GCScene.h"
#include "3D/Shaders/GCStandardSurface.h"
#include "3D/GCShadingGroup.h"
#include "3D/GCScreenRenderTarget.h"
#include "3D/GCTexture.h"
#include "3D/GCGeometry.h"
#include "object.h"

Viewport::Viewport(SPlugin &db) : SViewport(db.db().addChild<GCViewport>("SomeScene")), UISurface("Viewport", this, UISurface::Dock)
  {
  }

Viewport::~Viewport()
  {
  }

void Viewport::setScene(GCScene *s)
  {
  GCViewport *vp = viewport();
  vp->source.setPointed(s);
  _scene = s;
  setController(s);
  }

GCViewport *Viewport::viewport()
  {
  return SViewport::viewport();
  }

