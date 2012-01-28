#ifndef SCPATH_H
#define SCPATH_H

#include "QDeclarativeItem"
#include "QColor"
#include "QVector3D"
#include "QLinearGradient"
#include "QPen"

class ScPath : public QDeclarativeItem
  {
  Q_OBJECT
  Q_PROPERTY(QColor firstColour READ firstColour WRITE setFirstColour NOTIFY firstColourChanged)
  Q_PROPERTY(QColor lastColour READ lastColour WRITE setLastColour NOTIFY lastColourChanged)

  Q_PROPERTY(QPointF firstPoint READ firstPoint WRITE setFirstPoint NOTIFY firstPointChanged)
  Q_PROPERTY(QPointF lastPoint READ lastPoint WRITE setLastPoint NOTIFY lastPointChanged)

  Q_PROPERTY(QPointF firstNormal READ firstNormal WRITE setFirstNormal NOTIFY firstNormalChanged)
  Q_PROPERTY(QPointF lastNormal READ lastNormal WRITE setLastNormal NOTIFY lastNormalChanged)

  Q_PROPERTY(qreal width READ penWidth() WRITE setPenWidth NOTIFY widthChanged)

public:
  ScPath(QDeclarativeItem *parent = 0);

  Q_INVOKABLE float intersect(float x, float y, float tol) const;

  const QPointF &firstPoint() const;
  void setFirstPoint(const QPointF &c);
  const QPointF &lastPoint() const;
  void setLastPoint(const QPointF &c);

  const QPointF &firstNormal() const;
  void setFirstNormal(const QPointF &c);
  const QPointF &lastNormal() const;
  void setLastNormal(const QPointF &c);

  float penWidth() const;
  void setPenWidth(float w);

  const QColor &firstColour() const;
  void setFirstColour(const QColor &c);
  const QColor &lastColour() const;
  void setLastColour(const QColor &c);

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

signals:
  void firstColourChanged();
  void lastColourChanged();

  void firstPointChanged();
  void lastPointChanged();

  void firstNormalChanged();
  void lastNormalChanged();

  void widthChanged();

private:
  void updateRenderData();

  QColor _firstColour;
  QColor _lastColour;
  QPointF _firstPoint;
  QPointF _lastPoint;
  QPointF _firstNormal;
  QPointF _lastNormal;

  QPainterPath _path;
  QPen _pen;
  QLinearGradient _gradient;
  float _penWidth;
  };

#endif // SCPATH_H
