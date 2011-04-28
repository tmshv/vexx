#ifndef XABSTRACTCANVAS_H
#define XABSTRACTCANVAS_H

#include "XProperty"
#include "XAbstractRenderModel.h"
#include "X3DGlobal.h"

class XAbstractCanvasController;

class EKS3D_EXPORT XAbstractCanvas
  {
XProperties:
  XROProperty(XAbstractRenderModel *, model);
  XProperty(XAbstractCanvasController *, controller, setController);

public:
  XAbstractCanvas(XAbstractRenderModel *model=0, XAbstractCanvasController *controller=0);
  virtual ~XAbstractCanvas();

  virtual void update(XAbstractRenderModel::UpdateMode);
  virtual void paint();

  void setModel(XAbstractRenderModel *);

private:
  XAbstractRenderModel::Iterator *_iterator;
  };

#endif // XABSTRACTCANVAS_H
