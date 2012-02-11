#include "scpath.h"
#include "QPainter"
#include "XVector2D"

ScPath::ScPath(QDeclarativeItem *parent)
    : QDeclarativeItem(parent), _penWidth(1)
  {
  // need to disable this flag to draw inside a QDeclarativeItem
  setFlag(QGraphicsItem::ItemHasNoContents, false);
  }

float ScPath::intersect(float xIn, float yIn, float tol) const
  {
  QPainterPathStroker stroker;
  stroker.setWidth(width() * tol);
  QPainterPath path = stroker.createStroke(_path);

  QPointF pt(xIn, yIn);
  path.setFillRule(Qt::WindingFill);
  if(path.contains(pt))
    {
    XVector2D pos(x(), y());
    XVector2D fP(_firstPoint.x(), _firstPoint.y());
    XVector2D lP(_lastPoint.x(), _lastPoint.y());
    XVector2D offset = XVector2D(pt.x(), pt.y()) - (fP - pos);
    XVector2D vec = lP - fP;
    XVector2D delta = vec.normalized();

    float projectedLength = delta.dot(offset);
    qDebug() << "Path hit?" << projectedLength / vec.norm();
    return projectedLength / vec.norm();
    }

  return -HUGE_VAL;
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

  p->setPen(_pen);
  p->setBrush(Qt::transparent);

  p->drawPath(_path);

#define DEBUG_RENDER_PATHSx
#ifdef DEBUG_RENDER_PATHS
  p->setRenderHint(QPainter::Antialiasing, false);
  p->setPen(Qt::red);
  p->setBrush(Qt::transparent);
  p->drawRect(0, 0, width(), height());

  p->drawText(boundingRect(), QString::number((quint64)this, 16));
#endif

  p->setRenderHint(QPainter::Antialiasing, oldAA);
  }

void ScPath::updateRenderData()
  {
  QPointF to = _lastPoint;
  QPointF from = _firstPoint;

  float test = to.x() * 0.0f + to.y() * 0.0f + from.x() * 0.0f + from.y() * 0.0f;
  if(test != test)
    {
    _path = QPainterPath();
    update();
    return;
    }

  QPainterPath path;
  path.moveTo(from);
  path.cubicTo(from.x() + _firstNormal.x(), from.y() + _firstNormal.y(),
               to.x() + _lastNormal.x(), to.y() + _lastNormal.y(),
               to.x(), to.y());
  _path = path;

  QRectF rect = _path.boundingRect();

  setX(rect.left() - _penWidth/2);
  setY(rect.top() - _penWidth/2);

  setWidth(rect.width() + _penWidth);
  setHeight(rect.height() + _penWidth);

  _path.translate(-rect.left(), -rect.top());

  _gradient.setColorAt(0.0, _firstColour);
  _gradient.setColorAt(1.0, _lastColour);
  _gradient.setStart(from - pos());
  _gradient.setFinalStop(to - pos());

  QBrush br(_gradient);
  _pen.setBrush(br);
  _pen.setWidth(_penWidth);

  update();
  }
