#ifndef SCCONNECTORITEM_H
#define SCCONNECTORITEM_H

#include <QDeclarativeItem>
#include "XProperty"

class SProperty;
class ScPropertyItem;
class ScNodeDisplay;

class ScConnectorItem : public QDeclarativeItem
  {
  Q_OBJECT

  Q_PROPERTY(QPointF driverPoint READ driverPoint NOTIFY driverPointChanged)
  Q_PROPERTY(QPointF drivenPoint READ drivenPoint NOTIFY drivenPointChanged)
  Q_PROPERTY(QPointF driverNormal READ driverNormal NOTIFY driverNormalChanged)
  Q_PROPERTY(QPointF drivenNormal READ drivenNormal NOTIFY drivenNormalChanged)
  Q_PROPERTY(QColor driverColour READ driverColour NOTIFY colourChanged)
  Q_PROPERTY(QColor drivenColour READ drivenColour NOTIFY colourChanged)

  XROProperty(const SProperty *, driverProperty);
  XROProperty(const SProperty *, drivenProperty);
  XROProperty(ScPropertyItem *, driver);
  XROProperty(ScPropertyItem *, driven);

public:
  explicit ScConnectorItem(QDeclarativeItem *parent = 0);

  void setProperties(const SProperty *driver, const SProperty *driven);
  void setNodeDisplay(ScNodeDisplay *disp);
  void setDriverItem(ScPropertyItem *i);
  void setDrivenItem(ScPropertyItem *i);

  QPointF driverPoint() const;
  QPointF drivenPoint() const;
  QPointF driverNormal() const;
  QPointF drivenNormal() const;
  QColor driverColour() const;
  QColor drivenColour() const;

signals:
  void driverPointChanged();
  void drivenPointChanged();
  void driverNormalChanged();
  void drivenNormalChanged();
  void colourChanged();
  
public slots:
  
private:
  ScNodeDisplay *_display;
  };

#endif // SCCONNECTORITEM_H
