#ifndef XABSTRACTCANVASCONTROLLER_H
#define XABSTRACTCANVASCONTROLLER_H

#include "X3DGlobal.h"
#include "XProperty"
#include "QEvent"
#include "QPoint"

class XAbstractCanvas;

class EKS3D_EXPORT XAbstractCanvasController
  {
XProperties:
  XROProperty(XAbstractCanvas *, canvas);
  XROProperty(QPoint, lastKnownMousePosition);

public:
  enum MouseEventType
    {
    DoubleClick,
    Move,
    Press,
    Release
    };

  XAbstractCanvasController(XAbstractCanvas *canvas);

  enum Result
    {
    NotUsed = 0,
    Used = 1,
    NeedsUpdate = 2
    };

  virtual xuint32 maxNumberOfPasses(xuint32 modelMaxPasses) const { return 0; }
  virtual void paint(xuint32 pass) const { }

  int triggerMouseEvent(MouseEventType type,
                         QPoint point,
                         Qt::MouseButton triggerButton,
                         Qt::MouseButtons buttonsDown,
                         Qt::KeyboardModifiers modifiers);

protected:
  virtual int mouseEvent(MouseEventType type,
                          QPoint point,
                          Qt::MouseButton triggerButton,
                          Qt::MouseButtons buttonsDown,
                          Qt::KeyboardModifiers modifiers) { return false; }

  X_DISABLE_COPY(XAbstractCanvasController);
  };

#endif // XABSTRACTCANVASCONTROLLER_H
