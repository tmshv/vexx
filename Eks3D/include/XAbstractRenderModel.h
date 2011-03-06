#ifndef XABSTRACTRENDERMODEL_H
#define XABSTRACTRENDERMODEL_H

#include "XList"
#include "XProperty"
#include "X3DGlobal.h"

class XAbstractCanvas;
class XAbstractDelegate;

class EKS3D_EXPORT XAbstractRenderModel
  {
XProperties:
  XRORefProperty(XList<XAbstractCanvas*>, canvases);

public:
  class EKS3D_EXPORT Iterator
    {
  public:
    virtual ~Iterator();
    virtual bool next() = 0;
    };

  enum UpdateMode
    {
    RenderChange = 1<<0,
    BoundsChange = 1<<1 | RenderChange,
    TreeChange = 1<<2 | BoundsChange | RenderChange
    };

  virtual ~XAbstractRenderModel();

  void update(UpdateMode m) const;

  virtual Iterator *createIterator() const = 0;
  virtual void destroyIterator(Iterator *) const = 0;

  virtual void resetIterator(Iterator *) const = 0;

  virtual void paint(XAbstractCanvas *, const Iterator *, const XAbstractDelegate *) const = 0;
  virtual XAbstractDelegate *delegateFor(Iterator *) const = 0;

private:
  friend class XAbstractCanvas;
  };

#endif // XABSTRACTRENDERMODEL_H
