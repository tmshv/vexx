#ifndef SYPREVIEWVIEWER_H
#define SYPREVIEWVIEWER_H

#include "UISurface.h"
#include "X2DCanvas.h"
#include "XAbstractDelegate.h"
#include "GCShiftRenderModel.h"
#include "sentityweakpointer.h"
#include "QImage"

class X2DCanvas;

class SyPreviewViewer : public UISurface, private GCShiftRenderModel, private XAbstractDelegate
  {
public:
  SyPreviewViewer(SEntity *root);

  // GCSingleLevelRenderModel virtuals
  virtual const XAbstractDelegate *delegateFor(XAbstractRenderModel::Iterator *, const XAbstractCanvas *) const;

  // XAbstractDelegate virtuals
  virtual void paint(xuint32 pass, XAbstractCanvas *, const XAbstractRenderModel::Iterator *, const XAbstractRenderModel *) const;

private:
  X2DCanvas *_canvas;
  XSimple2DCanvasController _controller;
  };

#endif // SYPREVIEWVIEWER_H
