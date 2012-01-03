#ifndef SCELLIPSE_H
#define SCELLIPSE_H

#include "scbrush.h"
#include "QDeclarativeItem"
#include "QColor"

class ScEllipse : public QDeclarativeItem
  {
  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
  Q_PROPERTY(ScGradient *gradient READ gradient WRITE setGradient)
  Q_PROPERTY(ScPen * border READ border CONSTANT)

public:
  ScEllipse(QDeclarativeItem *parent=0);
  ~ScEllipse();

  QColor color() const;
  void setColor(const QColor &);

  ScPen *border();

  ScGradient *gradient() const;
  void setGradient(ScGradient *gradient);

  QRectF boundingRect() const;

  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

Q_SIGNALS:
  void colorChanged();

private Q_SLOTS:
  void doUpdate();

private:
  void generateBorderedEllipse();

  void drawEllipse(QPainter &painter);
  ScPen *getPen();
  void setPaintMargin(qreal margin);

  QColor _color;
  ScGradient *_gradient;
  ScPen *_pen;
  qreal _paintmargin;
  QPixmap _rectImage;

  Q_DISABLE_COPY(ScEllipse)
  };

#endif // SCELLIPSE_H
