#include "sentity.h"
#include "QStringList"
#include "sdatabase.h"

S_IMPLEMENT_PROPERTY(SEntity)

const SPropertyInformation *SEntity::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SEntity>("SEntity");
  info->add(&SEntity::children, "children");
  return info;
  }

SEntity::SEntity()
  {
  }

void SEntity::reparent(SEntity *ent)
  {
  xAssert(parent() && parentEntity());

  parent()->moveProperty(&ent->children, this);
  }

bool SEntity::acceptsChild(const SEntity *) const
  {
  return true;
  }

bool SEntity::acceptsParent(const SEntity *) const
  {
  return true;
  }

SEntity *SEntity::parentEntity() const
  {
  if(parent())
    {
    return parent()->entity();
    }
  return 0;
  }

SEntity *SEntity::findChildEntity(const QString &n)
  {
  SProperty *prop = children.findChild(n);
  if(!prop)
    {
    return 0;
    }
  return prop->castTo<SEntity>();
  }

const SEntity *SEntity::findChildEntity(const QString &n) const
  {
  const SProperty *prop = children.findChild(n);
  if(!prop)
    {
    return 0;
    }
  return prop->castTo<SEntity>();
  }

SEntity *SEntity::firstChildEntity() const
  {
  if(children.firstChild())
    {
    return children.firstChild()->entity();
    }
  return 0;
  }

SEntity *SEntity::nextSiblingEntity() const
  {
  if(nextSibling())
    {
    return nextSibling()->entity();
    }
  return 0;
  }

void SEntity::saveProperty(const SProperty *p, SSaver &l)
  {
  SPropertyContainer::saveProperty(p, l);
  }

SProperty *SEntity::loadProperty(SPropertyContainer *p, SLoader &l)
  {
  return SPropertyContainer::loadProperty(p, l);
  }

void SEntity::addDirtyObserver(SDirtyObserver *in)
  {
  ObserverStruct s;
  s.mode = ObserverStruct::Dirty;
  s.observer = in;
  _observers << s;
  }

void SEntity::addTreeObserver(STreeObserver *in)
  {
  ObserverStruct s;
  s.mode = ObserverStruct::Tree;
  s.observer = in;
  _observers << s;
  }

void SEntity::addConnectionObserver(SConnectionObserver *in)
  {
  ObserverStruct s;
  s.mode = ObserverStruct::Connection;
  s.observer = in;
  _observers << s;
  }

void SEntity::removeDirtyObserver(SDirtyObserver *in)
  {
  for(int x=0; x<_observers.size(); ++x)
    {
    if(_observers[x].observer == in)
      {
      xAssert(_observers[x].mode == ObserverStruct::Dirty);
      _observers.removeAt(x);
      --x;
      }
    }
  }

void SEntity::removeTreeObserver(STreeObserver *in)
  {
  for(int x=0; x<_observers.size(); ++x)
    {
    if(_observers[x].observer == in)
      {
      xAssert(_observers[x].mode == ObserverStruct::Tree);
      _observers.removeAt(x);
      --x;
      }
    }
  }

void SEntity::removeConnectionObserver(SConnectionObserver *in)
  {
  for(int x=0; x<_observers.size(); ++x)
    {
    if(_observers[x].observer == in)
      {
      xAssert(_observers[x].mode == ObserverStruct::Connection);
      _observers.removeAt(x);
      --x;
      }
    }
  }

void SEntity::informDirtyObservers(SProperty *prop)
  {
  SProfileFunction
  foreach(const ObserverStruct &obs, _observers)
    {
    if(obs.mode == ObserverStruct::Dirty)
      {
      ((SDirtyObserver*)obs.observer)->onPropertyDirtied(prop);
      database()->currentBlockObserverList() << obs.observer;
      }
    }

  // todo: maybe inform up the tree?
  }

void SEntity::informTreeObservers(const SChange *event)
  {
  SProfileFunction
  foreach(const ObserverStruct &obs, _observers)
    {
    if(obs.mode == ObserverStruct::Tree)
      {
      ((STreeObserver*)obs.observer)->onTreeChange(event);
      database()->currentBlockObserverList() << obs.observer;
      }
    }

  SEntity *parentEnt = parentEntity();
  if(parentEnt)
    {
    parentEnt->informTreeObservers(event);
    }
  }

void SEntity::informConnectionObservers(const SChange *event)
  {
  SProfileFunction
  foreach(const ObserverStruct &obs, _observers)
    {
    if(obs.mode == ObserverStruct::Connection)
      {
      ((SConnectionObserver*)obs.observer)->onConnectionChange(event);
      database()->currentBlockObserverList() << obs.observer;
      }
    }
  }

