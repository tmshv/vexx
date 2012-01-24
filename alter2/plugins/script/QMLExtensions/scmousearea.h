#ifndef SCMOUSEAREA_H
#define SCMOUSEAREA_H

#include "QDeclarativeItem"
#include "QBasicTimer"

class ScMouseEvent : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int x READ x)
  Q_PROPERTY(int y READ y)
  Q_PROPERTY(int button READ button)
  Q_PROPERTY(int buttons READ buttons)
  Q_PROPERTY(int modifiers READ modifiers)
  Q_PROPERTY(bool wasHeld READ wasHeld)
  Q_PROPERTY(bool isClick READ isClick)
  Q_PROPERTY(bool accepted READ isAccepted WRITE setAccepted)

public:
  ScMouseEvent(int x, int y, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, bool isClick=false, bool wasHeld=false)
      : _x(x), _y(y), _button(button), _buttons(buttons), _modifiers(modifiers), _wasHeld(wasHeld), _isClick(isClick), _accepted(true)
    {
    }

  int x() const { return _x; }
  int y() const { return _y; }
  int button() const { return _button; }
  int buttons() const { return _buttons; }
  int modifiers() const { return _modifiers; }
  bool wasHeld() const { return _wasHeld; }
  bool isClick() const { return _isClick; }

  // only for internal usage
  void setX(int x) { _x = x; }
  void setY(int y) { _y = y; }

  bool isAccepted() { return _accepted; }
  void setAccepted(bool accepted) { _accepted = accepted; }

private:
  int _x;
  int _y;
  Qt::MouseButton _button;
  Qt::MouseButtons _buttons;
  Qt::KeyboardModifiers _modifiers;
  bool _wasHeld;
  bool _isClick;
  bool _accepted;
};

class ScDrag : public QObject
{
  Q_OBJECT
  Q_ENUMS(Axis)
  Q_PROPERTY(QGraphicsObject *target READ target WRITE setTarget NOTIFY targetChanged RESET resetTarget)
  Q_PROPERTY(Axis axis READ axis WRITE setAxis NOTIFY axisChanged)
  Q_PROPERTY(qreal minimumX READ xmin WRITE setXmin NOTIFY minimumXChanged)
  Q_PROPERTY(qreal maximumX READ xmax WRITE setXmax NOTIFY maximumXChanged)
  Q_PROPERTY(qreal minimumY READ ymin WRITE setYmin NOTIFY minimumYChanged)
  Q_PROPERTY(qreal maximumY READ ymax WRITE setYmax NOTIFY maximumYChanged)
  Q_PROPERTY(bool active READ active NOTIFY activeChanged)
  Q_PROPERTY(bool filterChildren READ filterChildren WRITE setFilterChildren NOTIFY filterChildrenChanged)

public:
  ScDrag(QObject *parent=0);
  ~ScDrag();

  QGraphicsObject *target() const;
  void setTarget(QGraphicsObject *);
  void resetTarget();

  enum Axis { XAxis=0x01, YAxis=0x02, XandYAxis=0x03 };
  Axis axis() const;
  void setAxis(Axis);

  qreal xmin() const;
  void setXmin(qreal);
  qreal xmax() const;
  void setXmax(qreal);
  qreal ymin() const;
  void setYmin(qreal);
  qreal ymax() const;
  void setYmax(qreal);

  bool active() const;
  void setActive(bool);

  bool filterChildren() const;
  void setFilterChildren(bool);

Q_SIGNALS:
  void targetChanged();
  void axisChanged();
  void minimumXChanged();
  void maximumXChanged();
  void minimumYChanged();
  void maximumYChanged();
  void activeChanged();
  void filterChildrenChanged();

private:
  QGraphicsObject *_target;
  Axis _axis;
  qreal _xmin;
  qreal _xmax;
  qreal _ymin;
  qreal _ymax;
  bool _active : 1;
  bool _filterChildren: 1;

  Q_DISABLE_COPY(ScDrag)
};

class ScMouseEvent;
class ScMouseArea : public QDeclarativeItem
{
  Q_OBJECT
  Q_PROPERTY(qreal mouseX READ mouseX NOTIFY mousePositionChanged)
  Q_PROPERTY(qreal mouseY READ mouseY NOTIFY mousePositionChanged)
  Q_PROPERTY(bool containsMouse READ hovered NOTIFY hoveredChanged)
  Q_PROPERTY(bool pressed READ pressed NOTIFY pressedChanged)
  Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
  Q_PROPERTY(Qt::MouseButtons pressedButtons READ pressedButtons NOTIFY pressedChanged)
  Q_PROPERTY(Qt::MouseButtons acceptedButtons READ acceptedButtons WRITE setAcceptedButtons NOTIFY acceptedButtonsChanged)
  Q_PROPERTY(bool hoverEnabled READ hoverEnabled WRITE setHoverEnabled NOTIFY hoverEnabledChanged)
  Q_PROPERTY(ScDrag *drag READ drag CONSTANT) //### add flicking to QDeclarativeDrag or add a QDeclarativeFlick ???
  Q_PROPERTY(bool preventStealing READ preventStealing WRITE setPreventStealing NOTIFY preventStealingChanged)

public:
  ScMouseArea(QDeclarativeItem *parent=0);
  ~ScMouseArea();

  qreal mouseX() const;
  qreal mouseY() const;

  bool isEnabled() const;
  void setEnabled(bool);

  bool hovered() const;
  bool pressed() const;

  Qt::MouseButtons pressedButtons() const;

  Qt::MouseButtons acceptedButtons() const;
  void setAcceptedButtons(Qt::MouseButtons buttons);

  bool hoverEnabled() const;
  void setHoverEnabled(bool h);

  ScDrag *drag();

  bool preventStealing() const;
  void setPreventStealing(bool prevent);

Q_SIGNALS:
  void hoveredChanged();
  void pressedChanged();
  void enabledChanged();
  void acceptedButtonsChanged();
  void hoverEnabledChanged();
  void positionChanged(ScMouseEvent *mouse);
  void mousePositionChanged(ScMouseEvent *mouse);
  void preventStealingChanged();

  void pressed(ScMouseEvent *mouse);
  void pressAndHold(ScMouseEvent *mouse);
  void released(ScMouseEvent *mouse);
  void clicked(ScMouseEvent *mouse);
  void doubleClicked(ScMouseEvent *mouse);
  void entered();
  void exited();
  void canceled();

protected:
  void setHovered(bool);
  bool setPressed(bool);

  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
#ifndef QT_NO_CONTEXTMENU
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
#endif // QT_NO_CONTEXTMENU
  bool sceneEvent(QEvent *);
  bool sendMouseEvent(QGraphicsSceneMouseEvent *event);
  bool sceneEventFilter(QGraphicsItem *i, QEvent *e);
  void timerEvent(QTimerEvent *event);

  virtual void geometryChanged(const QRectF &newGeometry,
                               const QRectF &oldGeometry);
  virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private:
  void handlePress();
  void handleRelease();

private:
  Q_DISABLE_COPY(ScMouseArea)

  void saveEvent(QGraphicsSceneMouseEvent *event);

  bool _absorb : 1;
  bool _hovered : 1;
  bool _pressed : 1;
  bool _longPress : 1;
  bool _moved : 1;
  bool _dragX : 1;
  bool _dragY : 1;
  bool _stealMouse : 1;
  bool _doubleClick : 1;
  bool _preventStealing : 1;
  ScDrag *_drag;
  QPointF _startScene;
  qreal _startX;
  qreal _startY;
  QPointF _lastPos;
  QPointF _lastScenePos;
  Qt::MouseButton _lastButton;
  Qt::MouseButtons _lastButtons;
  Qt::KeyboardModifiers _lastModifiers;
  QBasicTimer _pressAndHoldTimer;
};

QML_DECLARE_TYPE(ScDrag)
QML_DECLARE_TYPE(ScMouseArea)
QML_DECLARE_TYPE(ScMouseEvent)

#endif // SCMOUSEAREA_H
