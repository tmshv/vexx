#include "scconnectoritem.h"
#include "scpropertyitem.h"
#include "scnodedisplay.h"

ScConnectorItem::ScConnectorItem(QDeclarativeItem *parent) :
  QDeclarativeItem(parent)
  {
  _display = 0;
  _driver = 0;
  _driven = 0;
  _driverProperty = 0;
  _drivenProperty = 0;
  }

void ScConnectorItem::setProperties(const SProperty *driver, const SProperty *driven)
  {
  _driverProperty = driver;
  _drivenProperty = driven;
  }

void ScConnectorItem::setNodeDisplay(ScNodeDisplay *disp)
  {
  xAssert(_display == 0);
  _display = disp;
  }

void ScConnectorItem::setDriverItem(ScPropertyItem *driver)
  {
  if(_driver)
    {
    disconnect(_driver, SIGNAL(scenePositionChanged()), this, SIGNAL(driverPointChanged()));
    }

  _driver = driver;

  connect(driver, SIGNAL(scenePositionChanged()), this, SIGNAL(driverPointChanged()));
  emit driverPointChanged();
  emit colourChanged();
  }

void ScConnectorItem::setDrivenItem(ScPropertyItem *driven)
  {
  if(_driven)
    {
    disconnect(_driven, SIGNAL(scenePositionChanged()), this, SIGNAL(drivenPointChanged()));
    }

  _driven = driven;
  connect(driven, SIGNAL(scenePositionChanged()), this, SIGNAL(drivenPointChanged()));
  emit drivenPointChanged();
  emit colourChanged();
  }

QPointF ScConnectorItem::driverPoint() const
  {
  xAssert(_driver);
  return QGraphicsObject::mapFromItem(_driver, 0, 0) + _driver->driverPoint();
  }

QPointF ScConnectorItem::drivenPoint() const
  {
  xAssert(_driven);
  return QGraphicsObject::mapFromItem(_driven, 0, 0) + _driven->drivenPoint();
  }

QPointF ScConnectorItem::driverNormal() const
  {
  xAssert(_driver);
  return _driver->driverNormal();
  }

QPointF ScConnectorItem::drivenNormal() const
  {
  xAssert(_driven);
  return _driven->drivenNormal();
  }

QColor ScConnectorItem::driverColour() const
  {
  xAssert(_driver);
  return _driver->colour();
  }

QColor ScConnectorItem::drivenColour() const
  {
  xAssert(_driven);
  return _driven->colour();
  }
