#include "scmousearea.h"
#include "qquickitem_p.h"

#include <QtGui/qevent.h>
#include <QtCore/qbasictimer.h>

#include "qquickmousearea_p.h"
#include "qquickmousearea_p_p.h"
#include "qquickcanvas.h"
#include "qquickevents_p_p.h"
#include "qquickdrag_p.h"

#include <QtGui/qevent.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qstylehints.h>

#include <float.h>

class QQuickMouseEvent;
class QQuickMouseArea;
class QQuickMouseAreaPrivate : public QQuickItemPrivate
{
    Q_DECLARE_PUBLIC(QQuickMouseArea)

public:
    QQuickMouseAreaPrivate();
    ~QQuickMouseAreaPrivate();
    void init();

    void saveEvent(QMouseEvent *event);
    enum PropagateType{
        Click,
        DoubleClick,
        PressAndHold
    };
    void propagate(QQuickMouseEvent* event, PropagateType);
    bool propagateHelper(QQuickMouseEvent*, QQuickItem*,const QPointF &, PropagateType);

    bool isPressAndHoldConnected();
    bool isDoubleClickConnected();
    bool isClickConnected();

    bool absorb : 1;
    bool hovered : 1;
    bool pressed : 1;
    bool longPress : 1;
    bool moved : 1;
    bool dragX : 1;
    bool dragY : 1;
    bool stealMouse : 1;
    bool doubleClick : 1;
    bool preventStealing : 1;
    bool propagateComposedEvents : 1;
    QQuickDrag *drag;
    QPointF startScene;
    QPointF targetStartPos;
    QPointF lastPos;
    QDeclarativeNullableValue<QPointF> lastScenePos;
    Qt::MouseButton lastButton;
    Qt::MouseButtons lastButtons;
    Qt::KeyboardModifiers lastModifiers;
    QBasicTimer pressAndHoldTimer;
};

static const int PressAndHoldDelay = 800;

QQuickDrag::QQuickDrag(QObject *parent)
: QObject(parent), _target(0), _axis(XandYAxis), _xmin(-FLT_MAX),
_xmax(FLT_MAX), _ymin(-FLT_MAX), _ymax(FLT_MAX), _active(false), _filterChildren(false)
{
}

QQuickDrag::~QQuickDrag()
{
}

QQuickItem *QQuickDrag::target() const
{
    return _target;
}

void QQuickDrag::setTarget(QQuickItem *t)
{
    if (_target == t)
        return;
    _target = t;
    emit targetChanged();
}

void QQuickDrag::resetTarget()
{
    if (_target == 0)
        return;
    _target = 0;
    emit targetChanged();
}

QQuickDrag::Axis QQuickDrag::axis() const
{
    return _axis;
}

void QQuickDrag::setAxis(QQuickDrag::Axis a)
{
    if (_axis == a)
        return;
    _axis = a;
    emit axisChanged();
}

qreal QQuickDrag::xmin() const
{
    return _xmin;
}

void QQuickDrag::setXmin(qreal m)
{
    if (_xmin == m)
        return;
    _xmin = m;
    emit minimumXChanged();
}

qreal QQuickDrag::xmax() const
{
    return _xmax;
}

void QQuickDrag::setXmax(qreal m)
{
    if (_xmax == m)
        return;
    _xmax = m;
    emit maximumXChanged();
}

qreal QQuickDrag::ymin() const
{
    return _ymin;
}

void QQuickDrag::setYmin(qreal m)
{
    if (_ymin == m)
        return;
    _ymin = m;
    emit minimumYChanged();
}

qreal QQuickDrag::ymax() const
{
    return _ymax;
}

void QQuickDrag::setYmax(qreal m)
{
    if (_ymax == m)
        return;
    _ymax = m;
    emit maximumYChanged();
}

bool QQuickDrag::active() const
{
    return _active;
}

void QQuickDrag::setActive(bool drag)
{
    if (_active == drag)
        return;
    _active = drag;
    emit activeChanged();
}

bool QQuickDrag::filterChildren() const
{
    return _filterChildren;
}

void QQuickDrag::setFilterChildren(bool filter)
{
    if (_filterChildren == filter)
        return;
    _filterChildren = filter;
    emit filterChildrenChanged();
}

QQuickDragAttached *QQuickDrag::qmlAttachedProperties(QObject *obj)
{
    return new QQuickDragAttached(obj);
}

QQuickMouseAreaPrivate::QQuickMouseAreaPrivate()
: absorb(true), hovered(false), pressed(false), longPress(false),
  moved(false), stealMouse(false), doubleClick(false), preventStealing(false),
  propagateComposedEvents(false), drag(0)
{
}

QQuickMouseAreaPrivate::~QQuickMouseAreaPrivate()
{
    delete drag;
}

void QQuickMouseAreaPrivate::init()
{
    Q_Q(QQuickMouseArea);
    q->setAcceptedMouseButtons(Qt::LeftButton);
    q->setFiltersChildMouseEvents(true);
}

void QQuickMouseAreaPrivate::saveEvent(QMouseEvent *event)
{
    lastPos = event->localPos();
    lastScenePos = event->windowPos();
    lastButton = event->button();
    lastButtons = event->buttons();
    lastModifiers = event->modifiers();
}

bool QQuickMouseAreaPrivate::isPressAndHoldConnected()
{
    Q_Q(QQuickMouseArea);
    static int idx = QObjectPrivate::get(q)->signalIndex("pressAndHold(QQuickMouseEvent*)");
    return QObjectPrivate::get(q)->isSignalConnected(idx);
}

bool QQuickMouseAreaPrivate::isDoubleClickConnected()
{
    Q_Q(QQuickMouseArea);
    static int idx = QObjectPrivate::get(q)->signalIndex("doubleClicked(QQuickMouseEvent*)");
    return QObjectPrivate::get(q)->isSignalConnected(idx);
}

bool QQuickMouseAreaPrivate::isClickConnected()
{
    Q_Q(QQuickMouseArea);
    static int idx = QObjectPrivate::get(q)->signalIndex("clicked(QQuickMouseEvent*)");
    return QObjectPrivate::get(q)->isSignalConnected(idx);
}

void QQuickMouseAreaPrivate::propagate(QQuickMouseEvent* event, PropagateType t)
{
    Q_Q(QQuickMouseArea);
    if (!propagateComposedEvents)
        return;
    QPointF scenePos = q->mapToScene(QPointF(event->x(), event->y()));
    propagateHelper(event, canvas->rootItem(), scenePos, t);
}

bool QQuickMouseAreaPrivate::propagateHelper(QQuickMouseEvent *ev, QQuickItem *item,const QPointF &sp, PropagateType sig)
{
    //Based off of QQuickCanvas::deliverInitialMousePressEvent
    //But specific to MouseArea, so doesn't belong in canvas
    Q_Q(const QQuickMouseArea);
    QQuickItemPrivate *itemPrivate = QQuickItemPrivate::get(item);
    if (itemPrivate->opacity == 0.0)
        return false;

    if (itemPrivate->flags & QQuickItem::ItemClipsChildrenToShape) {
        QPointF p = item->mapFromScene(sp);
        if (!QRectF(0, 0, item->width(), item->height()).contains(p))
            return false;
    }

    QList<QQuickItem *> children = itemPrivate->paintOrderChildItems();
    for (int ii = children.count() - 1; ii >= 0; --ii) {
        QQuickItem *child = children.at(ii);
        if (!child->isVisible() || !child->isEnabled())
            continue;
        if (propagateHelper(ev, child, sp, sig))
            return true;
    }

    QQuickMouseArea* ma = qobject_cast<QQuickMouseArea*>(item);
    if (ma && ma != q && itemPrivate->acceptedMouseButtons & ev->button()) {
        switch (sig) {
        case Click:
            if (!ma->d_func()->isClickConnected())
                return false;
            break;
        case DoubleClick:
            if (!ma->d_func()->isDoubleClickConnected())
                return false;
            break;
        case PressAndHold:
            if (!ma->d_func()->isPressAndHoldConnected())
                return false;
            break;
        }
        QPointF p = item->mapFromScene(sp);
        if (QRectF(0, 0, item->width(), item->height()).contains(p)) {
            ev->setX(p.x());
            ev->setY(p.y());
            ev->setAccepted(true);//It is connected, they have to explicitly ignore to let it slide
            switch (sig) {
            case Click: emit ma->clicked(ev); break;
            case DoubleClick: emit ma->doubleClicked(ev); break;
            case PressAndHold: emit ma->pressAndHold(ev); break;
            }
            if (ev->isAccepted())
                return true;
        }
    }
    return false;

}

QQuickMouseArea::QQuickMouseArea(QQuickItem *parent)
  : QQuickItem(*(new QQuickMouseAreaPrivate), parent)
{
    Q_D(QQuickMouseArea);
    d->init();
}

QQuickMouseArea::~QQuickMouseArea()
{
}

qreal QQuickMouseArea::mouseX() const
{
    Q_D(const QQuickMouseArea);
    return d->lastPos.x();
}

qreal QQuickMouseArea::mouseY() const
{
    Q_D(const QQuickMouseArea);
    return d->lastPos.y();
}

bool QQuickMouseArea::isEnabled() const
{
    Q_D(const QQuickMouseArea);
    return d->absorb;
}

void QQuickMouseArea::setEnabled(bool a)
{
    Q_D(QQuickMouseArea);
    if (a != d->absorb) {
        d->absorb = a;
        emit enabledChanged();
    }
}

bool QQuickMouseArea::preventStealing() const
{
    Q_D(const QQuickMouseArea);
    return d->preventStealing;
}

void QQuickMouseArea::setPreventStealing(bool prevent)
{
    Q_D(QQuickMouseArea);
    if (prevent != d->preventStealing) {
        d->preventStealing = prevent;
        setKeepMouseGrab(d->preventStealing && d->absorb);
        emit preventStealingChanged();
    }
}

bool QQuickMouseArea::propagateComposedEvents() const
{
    Q_D(const QQuickMouseArea);
    return d->propagateComposedEvents;
}

void QQuickMouseArea::setPropagateComposedEvents(bool prevent)
{
    Q_D(QQuickMouseArea);
    if (prevent != d->propagateComposedEvents) {
        d->propagateComposedEvents = prevent;
        setKeepMouseGrab(d->propagateComposedEvents && d->absorb);
        emit propagateComposedEventsChanged();
    }
}

Qt::MouseButtons QQuickMouseArea::pressedButtons() const
{
    Q_D(const QQuickMouseArea);
    return d->lastButtons;
}

void QQuickMouseArea::mousePressEvent(QMouseEvent *event)
{
    Q_D(QQuickMouseArea);
    d->moved = false;
    d->stealMouse = d->preventStealing;
    if (!d->absorb)
        QQuickItem::mousePressEvent(event);
    else {
        d->longPress = false;
        d->saveEvent(event);
        if (d->drag) {
            d->dragX = drag()->axis() & QQuickDrag::XAxis;
            d->dragY = drag()->axis() & QQuickDrag::YAxis;
        }
        if (d->drag)
            d->drag->setActive(false);
        setHovered(true);
        d->startScene = event->windowPos();
        d->pressAndHoldTimer.start(PressAndHoldDelay, this);
        setKeepMouseGrab(d->stealMouse);
        event->setAccepted(setPressed(true));

    }
}

void QQuickMouseArea::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(QQuickMouseArea);
    if (!d->absorb) {
        QQuickItem::mouseMoveEvent(event);
        return;
    }

    d->saveEvent(event);

    // ### we should skip this if these signals aren't used
    // ### can GV handle this for us?
    bool contains = boundingRect().contains(d->lastPos);
    if (d->hovered && !contains)
        setHovered(false);
    else if (!d->hovered && contains)
        setHovered(true);

    if (d->drag && d->drag->target()) {
        if (!d->moved) {
            d->targetStartPos = d->drag->target()->parentItem()
                    ? d->drag->target()->parentItem()->mapToScene(d->drag->target()->pos())
                    : d->drag->target()->pos();
        }

        QPointF startLocalPos;
        QPointF curLocalPos;
        if (drag()->target()->parentItem()) {
            startLocalPos = drag()->target()->parentItem()->mapFromScene(d->startScene);
            curLocalPos = drag()->target()->parentItem()->mapFromScene(event->windowPos());
        } else {
            startLocalPos = d->startScene;
            curLocalPos = event->windowPos();
        }

        const int dragThreshold = qApp->styleHints()->startDragDistance();
        qreal dx = qAbs(curLocalPos.x() - startLocalPos.x());
        qreal dy = qAbs(curLocalPos.y() - startLocalPos.y());

        if (keepMouseGrab() && d->stealMouse && !d->drag->active())
            d->drag->setActive(true);

        QPointF startPos = d->drag->target()->parentItem()
                ? d->drag->target()->parentItem()->mapFromScene(d->targetStartPos)
                : d->targetStartPos;

        QPointF dragPos = d->drag->target()->pos();

        if (d->dragX && d->drag->active()) {
            qreal x = (curLocalPos.x() - startLocalPos.x()) + startPos.x();
            if (x < drag()->xmin())
                x = drag()->xmin();
            else if (x > drag()->xmax())
                x = drag()->xmax();
            dragPos.setX(x);
        }
        if (d->dragY && d->drag->active()) {
            qreal y = (curLocalPos.y() - startLocalPos.y()) + startPos.y();
            if (y < drag()->ymin())
                y = drag()->ymin();
            else if (y > drag()->ymax())
                y = drag()->ymax();
            dragPos.setY(y);
        }
        d->drag->target()->setPos(dragPos);

        if (!keepMouseGrab()) {
            if ((!d->dragY && dy < dragThreshold && d->dragX && dx > dragThreshold)
                || (!d->dragX && dx < dragThreshold && d->dragY && dy > dragThreshold)
                || (d->dragX && d->dragY && (dx > dragThreshold || dy > dragThreshold))) {
                setKeepMouseGrab(true);
                d->stealMouse = true;
            }
        }

        d->moved = true;
    }
    QQuickMouseEvent me(d->lastPos.x(), d->lastPos.y(), d->lastButton, d->lastButtons, d->lastModifiers, false, d->longPress);
    emit mouseXChanged(&me);
    me.setPosition(d->lastPos);
    emit mouseYChanged(&me);
    me.setPosition(d->lastPos);
    emit positionChanged(&me);
}

void QQuickMouseArea::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QQuickMouseArea);
    d->stealMouse = false;
    if (!d->absorb) {
        QQuickItem::mouseReleaseEvent(event);
    } else {
        d->saveEvent(event);
        setPressed(false);
        if (d->drag)
            d->drag->setActive(false);
        // If we don't accept hover, we need to reset containsMouse.
        if (!acceptHoverEvents())
            setHovered(false);
        QQuickCanvas *c = canvas();
        if (c && c->mouseGrabberItem() == this)
            ungrabMouse();
        setKeepMouseGrab(false);

    }
    d->doubleClick = false;
}

void QQuickMouseArea::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_D(QQuickMouseArea);
    if (d->absorb) {
        d->saveEvent(event);
        QQuickMouseEvent me(d->lastPos.x(), d->lastPos.y(), d->lastButton, d->lastButtons, d->lastModifiers, true, false);
        me.setAccepted(d->isDoubleClickConnected());
        emit this->doubleClicked(&me);
        if (!me.isAccepted())
            d->propagate(&me, QQuickMouseAreaPrivate::DoubleClick);
        d->doubleClick = d->isDoubleClickConnected() || me.isAccepted();
    }
    QQuickItem::mouseDoubleClickEvent(event);
}

void QQuickMouseArea::hoverEnterEvent(QHoverEvent *event)
{
    Q_D(QQuickMouseArea);
    if (!d->absorb) {
        QQuickItem::hoverEnterEvent(event);
    } else {
        d->lastPos = event->posF();
        d->lastModifiers = event->modifiers();
        setHovered(true);
        QQuickMouseEvent me(d->lastPos.x(), d->lastPos.y(), Qt::NoButton, Qt::NoButton, d->lastModifiers, false, false);
        emit mouseXChanged(&me);
        me.setPosition(d->lastPos);
        emit mouseYChanged(&me);
        me.setPosition(d->lastPos);
    }
}

void QQuickMouseArea::hoverMoveEvent(QHoverEvent *event)
{
    Q_D(QQuickMouseArea);
    if (!d->absorb) {
        QQuickItem::hoverMoveEvent(event);
    } else {
        d->lastPos = event->posF();
        d->lastModifiers = event->modifiers();
        QQuickMouseEvent me(d->lastPos.x(), d->lastPos.y(), Qt::NoButton, Qt::NoButton, d->lastModifiers, false, false);
        emit mouseXChanged(&me);
        me.setPosition(d->lastPos);
        emit mouseYChanged(&me);
        me.setPosition(d->lastPos);
        emit positionChanged(&me);
    }
}

void QQuickMouseArea::hoverLeaveEvent(QHoverEvent *event)
{
    Q_D(QQuickMouseArea);
    if (!d->absorb)
        QQuickItem::hoverLeaveEvent(event);
    else
        setHovered(false);
}

void QQuickMouseArea::ungrabMouse()
{
    Q_D(QQuickMouseArea);
    if (d->pressed) {
        // if our mouse grab has been removed (probably by Flickable), fix our
        // state
        d->pressed = false;
        d->stealMouse = false;
        setKeepMouseGrab(false);
        emit canceled();
        emit pressedChanged();
        if (d->hovered) {
            d->hovered = false;
            emit hoveredChanged();
        }
    }
}

void QQuickMouseArea::mouseUngrabEvent()
{
    ungrabMouse();
}

bool QQuickMouseArea::sendMouseEvent(QMouseEvent *event)
{
    Q_D(QQuickMouseArea);
    QRectF myRect = mapRectToScene(QRectF(0, 0, width(), height()));

    QQuickCanvas *c = canvas();
    QQuickItem *grabber = c ? c->mouseGrabberItem() : 0;
    bool stealThisEvent = d->stealMouse;
    if ((stealThisEvent || myRect.contains(event->windowPos())) && (!grabber || !grabber->keepMouseGrab())) {
        QMouseEvent mouseEvent(event->type(), mapFromScene(event->windowPos()), event->windowPos(), event->screenPos(),
                               event->button(), event->buttons(), event->modifiers());
        mouseEvent.setAccepted(false);

        switch (event->type()) {
        case QEvent::MouseMove:
            mouseMoveEvent(&mouseEvent);
            break;
        case QEvent::MouseButtonPress:
            mousePressEvent(&mouseEvent);
            break;
        case QEvent::MouseButtonRelease:
            mouseReleaseEvent(&mouseEvent);
            break;
        default:
            break;
        }
        grabber = c->mouseGrabberItem();
        if (grabber && stealThisEvent && !grabber->keepMouseGrab() && grabber != this)
            grabMouse();

        return stealThisEvent;
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        if (d->pressed) {
            d->pressed = false;
            d->stealMouse = false;
            if (c && c->mouseGrabberItem() == this)
                ungrabMouse();
            emit canceled();
            emit pressedChanged();
            if (d->hovered) {
                d->hovered = false;
                emit hoveredChanged();
            }
        }
    }
    return false;
}

bool QQuickMouseArea::childMouseEventFilter(QQuickItem *i, QEvent *e)
{
    Q_D(QQuickMouseArea);
    if (!d->absorb || !isVisible() || !d->drag || !d->drag->filterChildren())
        return QQuickItem::childMouseEventFilter(i, e);
    switch (e->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseMove:
    case QEvent::MouseButtonRelease:
        return sendMouseEvent(static_cast<QMouseEvent *>(e));
    default:
        break;
    }

    return QQuickItem::childMouseEventFilter(i, e);
}

void QQuickMouseArea::timerEvent(QTimerEvent *event)
{
    Q_D(QQuickMouseArea);
    if (event->timerId() == d->pressAndHoldTimer.timerId()) {
        d->pressAndHoldTimer.stop();
        bool dragged = d->drag && d->drag->active();
        if (d->pressed && dragged == false && d->hovered == true) {
            d->longPress = true;
            QQuickMouseEvent me(d->lastPos.x(), d->lastPos.y(), d->lastButton, d->lastButtons, d->lastModifiers, false, d->longPress);
            me.setAccepted(d->isPressAndHoldConnected());
            emit pressAndHold(&me);
            if (!me.isAccepted())
                d->propagate(&me, QQuickMouseAreaPrivate::PressAndHold);
            if (!me.isAccepted()) // no one handled the long press - allow click
                d->longPress = false;
        }
    }
}

void QQuickMouseArea::windowDeactivateEvent()
{
    ungrabMouse();
    QQuickItem::windowDeactivateEvent();
}

void QQuickMouseArea::geometryChanged(const QRectF &newGeometry,
                                            const QRectF &oldGeometry)
{
    Q_D(QQuickMouseArea);
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    if (d->lastScenePos.isNull)
        d->lastScenePos = mapToScene(d->lastPos);
    else if (newGeometry.x() != oldGeometry.x() || newGeometry.y() != oldGeometry.y())
        d->lastPos = mapFromScene(d->lastScenePos);
}

void QQuickMouseArea::itemChange(ItemChange change, const ItemChangeData &value)
{
    Q_D(QQuickMouseArea);
    switch (change) {
    case ItemVisibleHasChanged:
        if (acceptHoverEvents() && d->hovered != (isVisible() && isUnderMouse()))
            setHovered(!d->hovered);
        break;
    default:
        break;
    }

    QQuickItem::itemChange(change, value);
}

bool QQuickMouseArea::hoverEnabled() const
{
    return acceptHoverEvents();
}

void QQuickMouseArea::setHoverEnabled(bool h)
{
    if (h == acceptHoverEvents())
        return;

    setAcceptHoverEvents(h);
    emit hoverEnabledChanged();
}

bool QQuickMouseArea::hovered() const
{
    Q_D(const QQuickMouseArea);
    return d->hovered;
}

bool QQuickMouseArea::pressed() const
{
    Q_D(const QQuickMouseArea);
    return d->pressed;
}

void QQuickMouseArea::setHovered(bool h)
{
    Q_D(QQuickMouseArea);
    if (d->hovered != h) {
        d->hovered = h;
        emit hoveredChanged();
        d->hovered ? emit entered() : emit exited();
    }
}

Qt::MouseButtons QQuickMouseArea::acceptedButtons() const
{
    return acceptedMouseButtons();
}

void QQuickMouseArea::setAcceptedButtons(Qt::MouseButtons buttons)
{
    if (buttons != acceptedMouseButtons()) {
        setAcceptedMouseButtons(buttons);
        emit acceptedButtonsChanged();
    }
}

bool QQuickMouseArea::setPressed(bool p)
{
    Q_D(QQuickMouseArea);
    bool dragged = d->drag && d->drag->active();
    bool isclick = d->pressed == true && p == false && dragged == false && d->hovered == true;

    if (d->pressed != p) {
        d->pressed = p;
        QQuickMouseEvent me(d->lastPos.x(), d->lastPos.y(), d->lastButton, d->lastButtons, d->lastModifiers, isclick, d->longPress);
        if (d->pressed) {
            if (!d->doubleClick)
                emit pressed(&me);
            me.setPosition(d->lastPos);
            emit mouseXChanged(&me);
            me.setPosition(d->lastPos);
            emit mouseYChanged(&me);
            emit pressedChanged();
        } else {
            emit released(&me);
            me.setPosition(d->lastPos);
            emit pressedChanged();
            if (isclick && !d->longPress && !d->doubleClick){
                me.setAccepted(d->isClickConnected());
                emit clicked(&me);
                if (!me.isAccepted())
                    d->propagate(&me, QQuickMouseAreaPrivate::Click);
            }
        }

        return me.isAccepted();
    }
    return false;
}

QQuickDrag *QQuickMouseArea::drag()
{
    Q_D(QQuickMouseArea);
    if (!d->drag)
        d->drag = new QQuickDrag;
    return d->drag;
}
