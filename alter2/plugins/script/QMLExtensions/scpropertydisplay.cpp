#include "scpropertydisplay.h"
#include "scpropertyitem.h"
#include "scconnectoritem.h"
#include "spropertycontainer.h"
#include "spropertyinformation.h"

ScPropertyDisplay::ScPropertyDisplay(QDeclarativeItem *parent) : QDeclarativeItem(parent)
  {
  _property = 0;
  _parentItem = 0;
  connect(this, SIGNAL(visibleChanged()), SLOT(updateConnectionParenting()));
  }

ScPropertyDisplay::~ScPropertyDisplay()
  {
  if(_parentItem)
    {
    _parentItem->setPropertyDisplay(0);
    }
  }

int ScPropertyDisplay::count() const
  {
  return _properties.size();
  }

ScPropertyItem *ScPropertyDisplay::propertyAt(int i)
  {
  if(i < _properties.size())
    {
    return _properties[i];
    }

  xAssertFail();
  return 0;
  }

void ScPropertyDisplay::updateConnectionParenting()
  {
  if(!isVisibleTo(0))
    {
    xAssert(_property);

    foreach(ScPropertyItem *i, _properties)
      {
      ScPropertyDisplay *disp = i->display();
      if(disp)
        {
        disp->updateConnectionParenting();
        }
      }

    foreach(ScPropertyItem *i, _properties)
      {
      QVector<ScConnectorItem*> drivens = i->drivens();
      QVector<ScConnectorItem*> drivers = i->drivers();
      i->clearConnections();

      _parentItem->addDrivens(drivens);
      _parentItem->addDrivers(drivers);
      }
    }
  else
    {
    SProperty *parentProp = _parentItem->property();
    for(int i = 0, s = _parentItem->drivens().size(); i < s; ++i)
      {
      ScConnectorItem *driven = _parentItem->drivens()[i];
      const SProperty *prop = driven->drivenProperty();
      if(parentProp != prop)
        {
        foreach(ScPropertyItem *childProp, _properties)
          {
          if(prop->isDescendedFrom(childProp->property()))
            {
            _parentItem->removeDriven(i);
            childProp->addDriven(driven);
            i--;
            s--;
            break;
            }
          }
        }
      }

    for(int i = 0, s = _parentItem->drivers().size(); i < s; ++i)
      {
      ScConnectorItem *driver = _parentItem->drivers()[i];
      const SProperty *prop = driver->driverProperty();
      if(parentProp != prop)
        {
        foreach(ScPropertyItem *childProp, _properties)
          {
          if(prop->isDescendedFrom(childProp->property()))
            {
            _parentItem->removeDriver(i);
            childProp->addDriver(driver);
            i--;
            s--;
            break;
            }
          }
        }
      }

    foreach(ScPropertyItem *i, _properties)
      {
      ScPropertyDisplay *disp = i->display();
      if(disp)
        {
        disp->updateConnectionParenting();
        }
      }
    }
  }

void ScPropertyDisplay::setRootItem(ScPropertyItem *p)
  {
  _parentItem = p;
  if(_parentItem)
    {
    _parentItem->setPropertyDisplay(this);
    }

  rebuild();
  updateConnectionParenting();
  }

ScPropertyItem *ScPropertyDisplay::rootItem() const
  {
  return _parentItem;
  }

QDeclarativeComponent *ScPropertyDisplay::propertyComponent() const
  {
  return _property;
  }

void ScPropertyDisplay::setPropertyComponent(QDeclarativeComponent *p)
  {
  _property = p;
  rebuild();
  }

void ScPropertyDisplay::clear()
  {
  foreach(ScPropertyItem *i, _properties)
    {
    i->deleteLater();
    }

  _properties.clear();
  }

void ScPropertyDisplay::rebuild()
  {
  clear();

  if(!_property || !_parentItem)
    {
    return;
    }

  SProperty *p = _parentItem->property();
  if(!p)
    {
    return;
    }

  SPropertyContainer *cont = p->castTo<SPropertyContainer>();
  if(!cont)
    {
    return;
    }

  for(SProperty *c = cont->firstChild(); c; c = c->nextSibling())
    {
    addProperty(c);
    }
  }

void ScPropertyDisplay::addProperty(SProperty *e)
  {
  xAssert(e);

  if(e->instanceInformation()->mode() == SPropertyInstanceInformation::Internal ||
     e->instanceInformation()->mode() == SPropertyInstanceInformation::InternalComputed)
    {
    return;
    }

  QObject *created = _property->beginCreate(_property->creationContext());
  if(!created)
    {
    qWarning() << _property->errorString();
    return;
    }

  ScPropertyItem* node = qobject_cast<ScPropertyItem*>(created);
  xAssert(node);
  if(!node)
    {
    delete created;
    qWarning() << "node property of NodeDisplay must contain a NodeItem at highest level";
    return;
    }

  _properties << node;
  node->setParentItem(parentItem());
  node->setProperty(e);

  _property->completeCreate();

  emit propertyAdded(node);
  }

bool ScPropertyDisplay::onPropertyAdded(SProperty *p)
  {
  xAssert(_parentItem);
  if(p->parent() == _parentItem->property())
    {
    addProperty(p);
    }

  foreach(ScPropertyItem *i, _properties)
    {
    if(i->onPropertyAdded(p))
      {
      return true;
      }
    }
  return false;
  }

bool ScPropertyDisplay::onPropertyRemoved(const SPropertyContainer *oldParent, SProperty *p)
  {
  xAssert(_parentItem);
  if(oldParent == _parentItem->property())
    {
    foreach(ScPropertyItem *i, _properties)
      {
      if(i->property() == p)
        {
        i->deleteLater();
        }
      }
    }

  foreach(ScPropertyItem *i, _properties)
    {
    if(i->onPropertyRemoved(oldParent, p))
      {
      return true;
      }
    }
  return false;
  }
