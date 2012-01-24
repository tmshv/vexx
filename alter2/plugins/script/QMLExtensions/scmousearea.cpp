#include "scmousearea.h"

#include "QDateTime"
#include "QBasicTimer"
#include "QGraphicsSceneEvent"
#include "QApplication"
#include "QGraphicsScene"

#include <QGraphicsSceneMouseEvent>

#include <float.h>

static const int PressAndHoldDelay = 800;

ScDrag::ScDrag(QObject *parent) : QObject(parent), _target(0), _axis(XandYAxis), _xmin(-FLT_MAX), _xmax(FLT_MAX),
  _ymin(-FLT_MAX), _ymax(FLT_MAX), _active(false), _filterChildren(false)
{
}

ScDrag::~ScDrag()
{
}

QGraphicsObject *ScDrag::target() const
{
    return _target;
}

void ScDrag::setTarget(QGraphicsObject *t)
{
    if (_target == t)
        return;
    _target = t;
    emit targetChanged();
}

void ScDrag::resetTarget()
{
    if (!_target)
        return;
    _target = 0;
    emit targetChanged();
}

ScDrag::Axis ScDrag::axis() const
{
    return _axis;
}

void ScDrag::setAxis(ScDrag::Axis a)
{
    if (_axis == a)
        return;
    _axis = a;
    emit axisChanged();
}

qreal ScDrag::xmin() const
{
    return _xmin;
}

void ScDrag::setXmin(qreal m)
{
    if (_xmin == m)
        return;
    _xmin = m;
    emit minimumXChanged();
}

qreal ScDrag::xmax() const
{
    return _xmax;
}

void ScDrag::setXmax(qreal m)
{
    if (_xmax == m)
        return;
    _xmax = m;
    emit maximumXChanged();
}

qreal ScDrag::ymin() const
{
    return _ymin;
}

void ScDrag::setYmin(qreal m)
{
    if (_ymin == m)
        return;
    _ymin = m;
    emit minimumYChanged();
}

qreal ScDrag::ymax() const
{
    return _ymax;
}

void ScDrag::setYmax(qreal m)
{
    if (_ymax == m)
        return;
    _ymax = m;
    emit maximumYChanged();
}

bool ScDrag::active() const
{
    return _active;
}

void ScDrag::setActive(bool drag)
{
    if (_active == drag)
        return;
    _active = drag;
    emit activeChanged();
}

bool ScDrag::filterChildren() const
{
    return _filterChildren;
}

void ScDrag::setFilterChildren(bool filter)
{
    if (_filterChildren == filter)
        return;
    _filterChildren = filter;
    emit filterChildrenChanged();
}

ScMouseArea::ScMouseArea(QDeclarativeItem *parent)
  : QDeclarativeItem(parent), _absorb(true), _hovered(false), _pressed(false), _longPress(false),
    _moved(false), _stealMouse(false), _doubleClick(false), _preventStealing(false), _drag(0)
{
  setAcceptedMouseButtons(Qt::LeftButton);
  setFiltersChildEvents(true);
}

ScMouseArea::~ScMouseArea()
{
  delete _drag;
}

void ScMouseArea::saveEvent(QGraphicsSceneMouseEvent *event)
{
  _lastPos = event->pos();
  _lastScenePos = event->scenePos();
  _lastButton = event->button();
  _lastButtons = event->buttons();
  _lastModifiers = event->modifiers();
}

qreal ScMouseArea::mouseX() const
{
  return _lastPos.x();
}

qreal ScMouseArea::mouseY() const
{
  return _lastPos.y();
}

bool ScMouseArea::isEnabled() const
{
  return _absorb;
}

void ScMouseArea::setEnabled(bool a)
{
  if (a != _absorb)
    {
    _absorb = a;
    emit enabledChanged();
    }
}

bool ScMouseArea::preventStealing() const
{
  return _preventStealing;
}

void ScMouseArea::setPreventStealing(bool prevent)
{
  if (prevent != _preventStealing) {
    _preventStealing = prevent;
    setKeepMouseGrab(_preventStealing && _absorb);
    emit preventStealingChanged();
  }
}

Qt::MouseButtons ScMouseArea::pressedButtons() const
{
  return _lastButtons;
}

void ScMouseArea::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  _moved = false;
  _stealMouse = _preventStealing;
  if (!_absorb)
    QDeclarativeItem::mousePressEvent(event);
  else {
    _longPress = false;
    saveEvent(event);
    if (_drag) {
      _dragX = drag()->axis() & ScDrag::XAxis;
      _dragY = drag()->axis() & ScDrag::YAxis;
    }
    if (_drag)
      _drag->setActive(false);
    setHovered(true);
    _startScene = event->scenePos();
    // we should only start timer if pressAndHold is connected to.
    _pressAndHoldTimer.start(PressAndHoldDelay, this);
    setKeepMouseGrab(_stealMouse);
    event->setAccepted(setPressed(true));
  }
}

void ScMouseArea::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!_absorb) {
        QDeclarativeItem::mouseMoveEvent(event);
        return;
    }

    saveEvent(event);

    // ### we should skip this if these signals aren't used
    // ### can GV handle this for us?
    bool contains = boundingRect().contains(_lastPos);
    if (_hovered && !contains)
        setHovered(false);
    else if (!_hovered && contains)
        setHovered(true);

    if (_drag && _drag->target()) {
        if (!_moved) {
            _startX = drag()->target()->x();
            _startY = drag()->target()->y();
        }

        QPointF startLocalPos;
        QPointF curLocalPos;
        if (drag()->target()->parentItem()) {
            startLocalPos = drag()->target()->parentItem()->mapFromScene(_startScene);
            curLocalPos = drag()->target()->parentItem()->mapFromScene(event->scenePos());
        } else {
            startLocalPos = _startScene;
            curLocalPos = event->scenePos();
        }

        const int dragThreshold = QApplication::startDragDistance();
        qreal dx = qAbs(curLocalPos.x() - startLocalPos.x());
        qreal dy = qAbs(curLocalPos.y() - startLocalPos.y());

        if (keepMouseGrab() &&_stealMouse)
            _drag->setActive(true);

        if (_dragX && _drag->active()) {
            qreal x = (curLocalPos.x() - startLocalPos.x()) + _startX;
            if (x < drag()->xmin())
                x = drag()->xmin();
            else if (x > drag()->xmax())
                x = drag()->xmax();
            drag()->target()->setX(x);
        }
        if (_dragY && _drag->active()) {
            qreal y = (curLocalPos.y() - startLocalPos.y()) + _startY;
            if (y < drag()->ymin())
                y = drag()->ymin();
            else if (y > drag()->ymax())
                y = drag()->ymax();
            drag()->target()->setY(y);
        }

        if (!keepMouseGrab()) {
            if ((!_dragY && dy < dragThreshold && _dragX && dx > dragThreshold)
                || (!_dragX && dx < dragThreshold && _dragY && dy > dragThreshold)
                || (_dragX && _dragY && (dx > dragThreshold || dy > dragThreshold))) {
                setKeepMouseGrab(true);
                _stealMouse = true;
            }
        }

        _moved = true;
    }
    ScMouseEvent me(_lastPos.x(), _lastPos.y(), _lastButton, _lastButtons, _lastModifiers, false, _longPress);
    emit mousePositionChanged(&me);
    me.setX(_lastPos.x());
    me.setY(_lastPos.y());
    emit positionChanged(&me);
}


void ScMouseArea::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    _stealMouse = false;
    if (!_absorb) {
        QDeclarativeItem::mouseReleaseEvent(event);
    } else {
        saveEvent(event);
        setPressed(false);
        if (_drag)
            _drag->setActive(false);
        // If we don't accept hover, we need to reset containsMouse.
        if (!acceptHoverEvents())
            setHovered(false);
        QGraphicsScene *s = scene();
        if (s && s->mouseGrabberItem() == this)
            ungrabMouse();
        setKeepMouseGrab(false);
    }
    _doubleClick = false;
}

void ScMouseArea::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (!_absorb) {
        QDeclarativeItem::mouseDoubleClickEvent(event);
    } else {
        //if (_isDoubleClickConnected())
            _doubleClick = true;
        saveEvent(event);
        ScMouseEvent me(_lastPos.x(), _lastPos.y(), _lastButton, _lastButtons, _lastModifiers, true, false);
        //me.setAccepted(_isDoubleClickConnected());
        emit doubleClicked(&me);
        QDeclarativeItem::mouseDoubleClickEvent(event);
    }
}

void ScMouseArea::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (!_absorb)
        QDeclarativeItem::hoverEnterEvent(event);
    else {
        _lastPos = event->pos();
        setHovered(true);
        ScMouseEvent me(_lastPos.x(), _lastPos.y(), Qt::NoButton, Qt::NoButton, event->modifiers(), false, false);
        emit mousePositionChanged(&me);
    }
}

void ScMouseArea::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!_absorb) {
        QDeclarativeItem::hoverMoveEvent(event);
    } else {
        _lastPos = event->pos();
        ScMouseEvent me(_lastPos.x(), _lastPos.y(), Qt::NoButton, Qt::NoButton, event->modifiers(), false, false);
        emit mousePositionChanged(&me);
        me.setX(_lastPos.x());
        me.setY(_lastPos.y());
        emit positionChanged(&me);
    }
}

void ScMouseArea::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!_absorb)
        QDeclarativeItem::hoverLeaveEvent(event);
    else
        setHovered(false);
}

#ifndef QT_NO_CONTEXTMENU
void ScMouseArea::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    bool acceptsContextMenuButton;
#if defined(Q_OS_SYMBIAN)
    // In Symbian a Long Tap on the screen will trigger. See QSymbianControl::HandleLongTapEventL().
    acceptsContextMenuButton = acceptedButtons() & Qt::LeftButton;
#elif defined(Q_WS_WINCE)
    // ### WinCE can trigger context menu event with a gesture in the left button or a
    // click with the right button. Since we have no way here to differentiate them when
    // event happens, accepting either of the them will block the event.
    acceptsContextMenuButton = acceptedButtons() & (Qt::LeftButton | Qt::RightButton);
#else
    acceptsContextMenuButton = acceptedButtons() & Qt::RightButton;
#endif

    if (isEnabled() && event->reason() == QGraphicsSceneContextMenuEvent::Mouse
        && acceptsContextMenuButton) {
        // Do not let the context menu event propagate to items behind.
        return;
    }

    QDeclarativeItem::contextMenuEvent(event);
}
#endif // QT_NO_CONTEXTMENU

bool ScMouseArea::sceneEvent(QEvent *event)
{
    bool rv = QDeclarativeItem::sceneEvent(event);
    if (event->type() == QEvent::UngrabMouse) {
        if (_pressed) {
            // if our mouse grab has been removed (probably by Flickable), fix our
            // state
            _pressed = false;
            _stealMouse = false;
            setKeepMouseGrab(false);
            emit canceled();
            emit pressedChanged();
            if (_hovered) {
                _hovered = false;
                emit hoveredChanged();
            }
        }
    }
    return rv;
}

bool ScMouseArea::sendMouseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsSceneMouseEvent mouseEvent(event->type());
    QRectF myRect = mapToScene(QRectF(0, 0, width(), height())).boundingRect();

    QGraphicsScene *s = scene();
    QDeclarativeItem *grabber = s ? qobject_cast<QDeclarativeItem*>(s->mouseGrabberItem()) : 0;
    bool stealThisEvent = _stealMouse;
    if ((stealThisEvent || myRect.contains(event->scenePos().toPoint())) && (!grabber || !grabber->keepMouseGrab())) {
        mouseEvent.setAccepted(false);
        for (int i = 0x1; i <= 0x10; i <<= 1) {
            if (event->buttons() & i) {
                Qt::MouseButton button = Qt::MouseButton(i);
                mouseEvent.setButtonDownPos(button, mapFromScene(event->buttonDownPos(button)));
            }
        }
        mouseEvent.setScenePos(event->scenePos());
        mouseEvent.setLastScenePos(event->lastScenePos());
        mouseEvent.setPos(mapFromScene(event->scenePos()));
        mouseEvent.setLastPos(mapFromScene(event->lastScenePos()));

        switch(mouseEvent.type()) {
        case QEvent::GraphicsSceneMouseMove:
            mouseMoveEvent(&mouseEvent);
            break;
        case QEvent::GraphicsSceneMousePress:
            mousePressEvent(&mouseEvent);
            break;
        case QEvent::GraphicsSceneMouseRelease:
            mouseReleaseEvent(&mouseEvent);
            break;
        default:
            break;
        }
        grabber = qobject_cast<QDeclarativeItem*>(s->mouseGrabberItem());
        if (grabber && stealThisEvent && !grabber->keepMouseGrab() && grabber != this)
            grabMouse();

        return stealThisEvent;
    }
    if (mouseEvent.type() == QEvent::GraphicsSceneMouseRelease) {
        if (_pressed) {
            _pressed = false;
            _stealMouse = false;
            if (s && s->mouseGrabberItem() == this)
                ungrabMouse();
            emit canceled();
            emit pressedChanged();
            if (_hovered) {
                _hovered = false;
                emit hoveredChanged();
            }
        }
    }
    return false;
}

bool ScMouseArea::sceneEventFilter(QGraphicsItem *i, QEvent *e)
{
    if (!_absorb || !isVisible() || !_drag || !_drag->filterChildren())
        return QDeclarativeItem::sceneEventFilter(i, e);
    switch (e->type()) {
    case QEvent::GraphicsSceneMousePress:
    case QEvent::GraphicsSceneMouseMove:
    case QEvent::GraphicsSceneMouseRelease:
        return sendMouseEvent(static_cast<QGraphicsSceneMouseEvent *>(e));
    default:
        break;
    }

    return QDeclarativeItem::sceneEventFilter(i, e);
}

void ScMouseArea::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _pressAndHoldTimer.timerId()) {
        _pressAndHoldTimer.stop();
        bool dragged = _drag && _drag->active();
        if (_pressed && dragged == false && _hovered == true) {
            _longPress = true;
            ScMouseEvent me(_lastPos.x(), _lastPos.y(), _lastButton, _lastButtons, _lastModifiers, false, _longPress);
            emit pressAndHold(&me);
        }
    }
}

void ScMouseArea::geometryChanged(const QRectF &newGeometry,
                                            const QRectF &oldGeometry)
{
    QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);

    if (_lastScenePos.isNull())
        _lastScenePos = mapToScene(_lastPos);
    else if (newGeometry.x() != oldGeometry.x() || newGeometry.y() != oldGeometry.y())
        _lastPos = mapFromScene(_lastScenePos);
}

QVariant ScMouseArea::itemChange(GraphicsItemChange change,
                                       const QVariant &value)
{
    switch (change) {
    case ItemVisibleHasChanged:
        if (acceptHoverEvents() && _hovered != (isVisible() && isUnderMouse()))
            setHovered(!_hovered);
        break;
    default:
        break;
    }

    return QDeclarativeItem::itemChange(change, value);
}

bool ScMouseArea::hoverEnabled() const
{
    return acceptHoverEvents();
}

void ScMouseArea::setHoverEnabled(bool h)
{
    if (h == acceptHoverEvents())
        return;

    setAcceptHoverEvents(h);
    emit hoverEnabledChanged();
    if (_hovered != isUnderMouse())
        setHovered(!_hovered);
}

bool ScMouseArea::hovered() const
{
    return _hovered;
}

bool ScMouseArea::pressed() const
{
    return _pressed;
}

void ScMouseArea::setHovered(bool h)
{
    if (_hovered != h) {
        _hovered = h;
        emit hoveredChanged();
        _hovered ? emit entered() : emit exited();
    }
}

Qt::MouseButtons ScMouseArea::acceptedButtons() const
{
    return acceptedMouseButtons();
}

void ScMouseArea::setAcceptedButtons(Qt::MouseButtons buttons)
{
    if (buttons != acceptedMouseButtons()) {
        setAcceptedMouseButtons(buttons);
        emit acceptedButtonsChanged();
    }
}

bool ScMouseArea::setPressed(bool p)
{
    bool dragged = _drag && _drag->active();
    bool isclick = _pressed == true && p == false && dragged == false && _hovered == true;

    if (_pressed != p) {
        _pressed = p;
        ScMouseEvent me(_lastPos.x(), _lastPos.y(), _lastButton, _lastButtons, _lastModifiers, isclick, _longPress);
        if (_pressed) {
            if (!_doubleClick)
                emit pressed(&me);
            me.setX(_lastPos.x());
            me.setY(_lastPos.y());
            emit mousePositionChanged(&me);
            emit pressedChanged();
        } else {
            emit released(&me);
            me.setX(_lastPos.x());
            me.setY(_lastPos.y());
            emit pressedChanged();
            if (isclick && !_longPress && !_doubleClick)
                emit clicked(&me);
        }

        return me.isAccepted();
    }
    return false;
}

ScDrag *ScMouseArea::drag()
{
    if (!_drag)
        _drag = new ScDrag;
    return _drag;
}
