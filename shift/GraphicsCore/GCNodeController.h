#ifndef GCNODECONTROLLER_H
#define GCNODECONTROLLER_H

#include "GCGlobal.h"
#include "X2DCanvas.h"
#include "XAbstractRenderModel.h"

class SEntity;
class GCAbstractNodeDelegate;

class GRAPHICSCORE_EXPORT GCNodeController : public QObject, public XSimple2DCanvasController
  {
  Q_OBJECT
public:
  GCNodeController(X2DCanvas *c);
  ~GCNodeController();

  virtual UsedFlags mouseEvent(MouseEventType type,
                          QPoint point,
                          Qt::MouseButton triggerButton,
                          Qt::MouseButtons buttonsDown,
                          Qt::KeyboardModifiers modifiers,
                          int orientation);
  virtual xuint32 maxNumberOfPasses(xuint32 modelMaxPasses) const;
  virtual void paint(xuint32 pass) const;

signals:
  void onContextMenu(QPoint);

private:
  XAbstractRenderModel::Iterator *_iterator;

  SEntity *_interactionEntity;
  xsize _interactionProperty;
  const GCAbstractNodeDelegate *_interactionDelegate;
  bool _connectingOutput;

  enum ControlMode
    {
    None,
    MovingEntity,
    ConnectingProperty,
    ConnectingEntity
    };
  ControlMode _controlMode;

  mutable xuint32 _connectionPass;
  };

#endif // GCNODECONTROLLER_H
