#ifndef X2DCANVAS_H
#define X2DCANVAS_H

#include "X3DGlobal.h"
#include "QWidget"
#include "XAbstractCanvas.h"

class QPainter;

class EKS3D_EXPORT X2DCanvas : public QWidget, public XAbstractCanvas
  {
  Q_OBJECT

XProperties:
  XProperty(QColor, backgroundColour, setBackgroundColour);
  XROProperty(QPainter *, currentPainter);
  XROProperty(QRect, region);
  XProperty(bool, antiAliasingEnabled, setAntiAliasingEnabled);

public:
  X2DCanvas(QWidget *parent=0);

  virtual void mouseDoubleClickEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);

  virtual void update(XAbstractRenderModel::UpdateMode);

protected:
  virtual void paintEvent(QPaintEvent *);
  };

#endif // X2DCANVAS_H
