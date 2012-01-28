#ifndef SCBRUSH_H
#define SCBRUSH_H

#include "QObject"
#include "QColor"
#include "QGradient"
#include "QDeclarativeListProperty"

class ScPen : public QObject
  {
  Q_OBJECT
  Q_PROPERTY(int width READ width WRITE setWidth NOTIFY penChanged)
  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY penChanged)

public:
  ScPen(QObject *parent=0)
      : QObject(parent), _width(1), _color("#000000"), _valid(false)
    {
    }

  int width() const { return _width; }
  void setWidth(int w);

  QColor color() const { return _color; }
  void setColor(const QColor &c);

  bool isValid() { return _valid; }

Q_SIGNALS:
  void penChanged();

private:
  int _width;
  QColor _color;
  bool _valid;
  };

class ScGradientStop : public QObject
  {
  Q_OBJECT
  Q_PROPERTY(qreal position READ position WRITE setPosition)
  Q_PROPERTY(QColor color READ color WRITE setColor)

public:
  ScGradientStop(QObject *parent=0) : QObject(parent)
    {
    }

  qreal position() const { return m_position; }
  void setPosition(qreal position) { m_position = position; updateGradient(); }

  QColor color() const { return m_color; }
  void setColor(const QColor &color) { m_color = color; updateGradient(); }

private:
  void updateGradient();

private:
  qreal m_position;
  QColor m_color;
  };

class ScGradient : public QObject
  {
  Q_OBJECT
  Q_PROPERTY(QDeclarativeListProperty<ScGradientStop> stops READ stops)
  Q_CLASSINFO("DefaultProperty", "stops")

public:
  ScGradient(QObject *parent=0) : QObject(parent), m_gradient(0) {}
  ~ScGradient() { delete m_gradient; }

  QDeclarativeListProperty<ScGradientStop> stops() { return QDeclarativeListProperty<ScGradientStop>(this, m_stops); }

  const QGradient *gradient() const;

Q_SIGNALS:
  void updated();

private:
  void doUpdate();

private:
  QList<ScGradientStop *> m_stops;
  mutable QGradient *m_gradient;
  friend class ScGradientStop;
  };

#endif // SCBRUSH_H
