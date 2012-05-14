#ifndef SCPROPERTYITEM_H
#define SCPROPERTYITEM_H

#include <QDeclarativeItem>
#include "XProperty"
#include "sobserver.h"
#include "scpropertydisplay.h"
#include "spropertycontainer.h"

class SProperty;
class ScConnectorItem;

class ScPropertyItem : public QDeclarativeItem
  {
  Q_OBJECT

  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QColor colour READ colour CONSTANT)
  Q_PROPERTY(QString mode READ mode CONSTANT)
  Q_PROPERTY(bool hasChildren READ hasChildren NOTIFY hasChildrenChanged)
  Q_PROPERTY(QPointF driverPoint READ driverPoint WRITE setDriverPoint)
  Q_PROPERTY(QPointF drivenPoint READ drivenPoint WRITE setDrivenPoint)
  Q_PROPERTY(QPointF driverNormal READ driverNormal WRITE setDriverNormal)
  Q_PROPERTY(QPointF drivenNormal READ drivenNormal WRITE setDrivenNormal)
  Q_PROPERTY(bool deletable READ deletable CONSTANT)
  Q_PROPERTY(SProperty * property READ property CONSTANT)

  XProperty(SProperty *, property, setProperty);
  XProperty(QPointF, driverPoint, setDriverPoint);
  XProperty(QPointF, drivenPoint, setDrivenPoint);
  XProperty(QPointF, driverNormal, setDriverNormal);
  XProperty(QPointF, drivenNormal, setDrivenNormal);

public:
  explicit ScPropertyItem(QDeclarativeItem *parent = 0);

  virtual const QString& name() const;
  virtual void setName(const QString& );

  Q_INVOKABLE void remove();
  bool deletable() const;

  bool hasChildren() const;
  QString mode() const;
  QColor colour() const;

  void setPropertyDisplay(ScPropertyDisplay *d);

  bool onPropertyAdded(SProperty *);
  bool onPropertyRemoved(const SPropertyContainer *oldParent, SProperty *);

  ScPropertyDisplay *display() { return _display; }

  void addDriver(ScConnectorItem *);
  void addDriven(ScConnectorItem *);
  void removeDriver(ScConnectorItem *);
  void removeDriven(ScConnectorItem *);
  void removeDriver(int i);
  void removeDriven(int i);
  void addDrivers(const QVector<ScConnectorItem *> &);
  void addDrivens(const QVector<ScConnectorItem *> &);

  void clearConnections();
  const QVector <ScConnectorItem*>& drivers() const { return _drivers; }
  const QVector <ScConnectorItem*>& drivens() const { return _drivens; }

signals:
  void nameChanged();
  void hasChildrenChanged();
  void scenePositionChanged();

private:
  void setupConnection(ScConnectorItem *c) const;
  QVariant itemChange(GraphicsItemChange, const QVariant &);

  ScPropertyDisplay *_display;

  QVector <ScConnectorItem*> _drivers;
  QVector <ScConnectorItem*> _drivens;
  };

#endif // SCPROPERTYITEM_H
