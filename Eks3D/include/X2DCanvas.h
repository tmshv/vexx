#ifndef X2DCANVAS_H
#define X2DCANVAS_H

#include "X3DGlobal.h"
#include "QWidget"
#include "XAbstractCanvas.h"
#include "XAbstractCanvasController.h"
#include "QMouseEvent"


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

  X_CANVAS_GENERAL_MOUSEHANDLERS()

  virtual void update(XAbstractRenderModel::UpdateMode);

protected:
  virtual void paintEvent(QPaintEvent *);
  };

class EKS3D_EXPORT XSimple2DCanvasController : public XAbstractCanvasController
  {
public:
  XSimple2DCanvasController(X2DCanvas *canvas);
  virtual int mouseEvent(MouseEventType type,
                          QPoint point,
                          Qt::MouseButton triggerButton,
                          Qt::MouseButtons buttonsDown,
                          Qt::KeyboardModifiers modifiers);
  };

#endif // X2DCANVAS_H
