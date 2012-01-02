#include "scpath.h"
#include "QPainter"

ScPath::ScPath(QDeclarativeItem *parent)
    : QDeclarativeItem(parent), _penWidth(1)
  {
  // need to disable this flag to draw inside a QDeclarativeItem
  setFlag(QGraphicsItem::ItemHasNoContents, false);
  }

const QPointF &ScPath::firstPoint() const
  {
  return _firstPoint;
  }

void ScPath::setFirstPoint(const QPointF &c)
  {
  _firstPoint = c;
  updateRenderData();
  emit firstPointChanged();
  }

const QPointF &ScPath::lastPoint() const
  {
  return _lastPoint;
  }

void ScPath::setLastPoint(const QPointF &c)
  {
  _lastPoint = c;
  updateRenderData();
  emit lastPointChanged();
  }

const QPointF &ScPath::firstNormal() const
  {
  return _firstNormal;
  }

void ScPath::setFirstNormal(const QPointF &c)
  {
  _firstNormal = c;
  updateRenderData();
  emit firstNormalChanged();
  }

const QPointF &ScPath::lastNormal() const
  {
  return _lastNormal;
  }

void ScPath::setLastNormal(const QPointF &c)
  {
  _lastNormal = c;
  updateRenderData();
  emit lastNormalChanged();
  }

float ScPath::penWidth() const
  {
  return _penWidth;
  }

void ScPath::setPenWidth(float w)
  {
  _penWidth = qMax(w, 1.0f);
  updateRenderData();
  emit widthChanged();
  }

const QColor &ScPath::firstColour() const
  {
  return _firstColour;
  }

void ScPath::setFirstColour(const QColor &c)
  {
  _firstColour = c;
  updateRenderData();
  emit firstColourChanged();
  }

const QColor &ScPath::lastColour() const
  {
  return _lastColour;
  }

void ScPath::setLastColour(const QColor &c)
  {
  _lastColour = c;
  updateRenderData();
  emit lastColourChanged();
  }

void ScPath::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
  {
  bool oldAA = p->testRenderHint(QPainter::Antialiasing);
  p->setRenderHints(QPainter::Antialiasing, true);

  p->setPen(Qt::transparent);
  p->setBrush(_gradient);

  p->drawPolygon(_polygon, Qt::WindingFill);

  p->setRenderHint(QPainter::Antialiasing, oldAA);
  }

void ScPath::updateRenderData()
  {
  QRectF rect(_firstPoint, _lastPoint);
  QRectF rect2(rect.topLeft() + _firstNormal, rect.bottomRight() + _lastNormal);

  QRectF final(rect.united(rect2));

  setX(final.left() - _penWidth/2);
  setY(final.top() - _penWidth/2);

  setWidth(final.width() + _penWidth);
  setHeight(final.height() + _penWidth);

  QPointF to = _lastPoint - pos();
  QPointF from = _firstPoint - pos();

  float test = to.x() * 0.0f + to.y() * 0.0f + from.x() * 0.0f + from.y() * 0.0f;
  if(test != test)
    {
    _polygon = QPolygonF();
    update();
    return;
    }

  QPainterPath path;
  path.moveTo(from);
  path.cubicTo(from.x() + _firstNormal.x(), from.y() + _firstNormal.y(),
               to.x() + _lastNormal.x(), to.y() + _lastNormal.y(),
               to.x(), to.y());

  QPainterPathStroker stroker;
  Q_ASSERT(_penWidth >= 1.0f);
  stroker.setWidth(_penWidth);

  QPainterPath strokedPath = stroker.createStroke(path);

  _polygon = strokedPath.toFillPolygon(QTransform());

  _gradient.setStart(from);
  _gradient.setFinalStop(to);
  _gradient.setColorAt(0.0, _firstColour);
  _gradient.setColorAt(1.0, _lastColour);

  update();
  }
