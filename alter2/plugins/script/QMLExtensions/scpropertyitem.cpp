#include "scpropertyitem.h"
#include "sinterfaces.h"
#include "spropertyinformation.h"
#include "scconnectoritem.h"

ScPropertyItem::ScPropertyItem(QDeclarativeItem *parent) :
  QDeclarativeItem(parent)
  {
  _property = 0;
  _display = 0;

  setFlag(ItemSendsScenePositionChanges);
  }

const QString& ScPropertyItem::name() const
  {
  xAssert(_property);
  return _property->name();
  }

void ScPropertyItem::setName(const QString& n)
  {
  xAssert(_property);
  _property->setName(n);

  // todo, other changes, obsrver?
  emit nameChanged();
  }

bool ScPropertyItem::hasChildren() const
  {
  SPropertyContainer *cont = property()->castTo<SPropertyContainer>();
  if(cont)
    {
    return cont->size() != 0;
    }
  return 0;
  }

QString ScPropertyItem::mode() const
  {
  return property()->instanceInformation()->modeString();
  }

QColor ScPropertyItem::colour() const
  {
  const SPropertyColourInterface *interface = property()->interface<SPropertyColourInterface>();
  if(interface)
    {
    return interface->colour(property()).toLDRColour();
    }
  return QColor();
  }

void ScPropertyItem::setPropertyDisplay(ScPropertyDisplay *d)
  {
  xAssert(!d || !_display);

  _display = d;
  }

bool ScPropertyItem::onPropertyAdded(SProperty *p)
  {
  SPropertyContainer *c = property()->castTo<SPropertyContainer>();
  if(c)
    {
    if(c->size() == 1)
      {
      emit hasChildrenChanged();
      }
    }

  if(_display)
    {
    return _display->onPropertyAdded(p);
    }

  return false;
  }

bool ScPropertyItem::onPropertyRemoved(const SPropertyContainer *c, SProperty *p)
  {
  SPropertyContainer *cont = property()->castTo<SPropertyContainer>();
  if(cont)
    {
    if(cont->size() == 0)
      {
      emit hasChildrenChanged();
      }
    }

  if(_display)
    {
    return _display->onPropertyRemoved(c, p);
    }

  return false;
  }

void ScPropertyItem::addDriver(ScConnectorItem *a)
  {
  _drivers << a;
  a->setDriverItem(this);
  setupConnection(a);
  }

void ScPropertyItem::addDriven(ScConnectorItem *a)
  {
  _drivens << a;
  a->setDrivenItem(this);
  setupConnection(a);
  }

void ScPropertyItem::removeDriver(int i)
  {
  _drivers.erase(_drivers.begin() + i);
  }

void ScPropertyItem::removeDriven(int i)
  {
  _drivens.erase(_drivens.begin() + i);
  }

void ScPropertyItem::addDrivers(const QVector<ScConnectorItem *> &a)
  {
  foreach(ScConnectorItem *c, a)
    {
    c->setDriverItem(this);
    setupConnection(c);
    }

  _drivers << a;
  }

void ScPropertyItem::addDrivens(const QVector<ScConnectorItem *> &a)
  {
  foreach(ScConnectorItem *c, a)
    {
    c->setDrivenItem(this);
    setupConnection(c);
    }

  _drivens << a;
  }

void ScPropertyItem::setupConnection(ScConnectorItem *c) const
  {
  ScPropertyItem *driver = c->driver();
  ScPropertyItem *driven = c->driven();
  foreach(ScConnectorItem *i, _drivens)
    {
    if(i != c && driver == i->driver())
      {
      c->setVisible(false);
      return;
      }
    }

  foreach(ScConnectorItem *i, _drivers)
    {
    if(i != c && driven == i->driven())
      {
      c->setVisible(false);
      return;
      }
    }

  c->setVisible(true);
  }

void ScPropertyItem::clearConnections()
  {
  _drivens.clear();
  _drivers.clear();
  }

QVariant ScPropertyItem::itemChange(GraphicsItemChange c, const QVariant &v)
  {
  if(c == ItemScenePositionHasChanged)
    {
    emit scenePositionChanged();
    }
  return QDeclarativeItem::itemChange(c, v);
  }
