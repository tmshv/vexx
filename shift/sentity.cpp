#include "sentity.h"
#include "QStringList"
#include "sdatabase.h"

S_ENTITY_DEFINITION(SEntity, SPropertyContainer)
  S_PROPERTY_DEFINITION(SPropertyArray, children)
S_ENTITY_END_DEFINITION(SEntity, SPropertyContainer)

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

void SEntity::saveEntity(const SProperty *p, SPropertyData &d, SPropertyData::Mode m)
  {
  saveContainer(p, d, m);
  }

void SEntity::loadEntity(SProperty *p, const SPropertyData &d, xuint32 v, SPropertyData::Mode m, SLoader &l)
  {
  loadContainer(p, d, v, m, l);
  }

void SEntity::addDataObserver(SDataObserver *in)
  {
  ObserverStruct s;
  s.mode = ObserverStruct::Data;
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

void SEntity::removeDataObserver(SDataObserver *in)
  {
  for(int x=0; x<_observers.size(); ++x)
    {
    if(_observers[x].observer == in)
      {
      xAssert(_observers[x].mode == ObserverStruct::Data);
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

void SEntity::informDataObservers(int m, const DataChange *event, SObservers &obsList)
  {
  SProfileFunction
  foreach(const ObserverStruct &obs, _observers)
    {
    if(obs.mode == ObserverStruct::Data)
      {
      ((SDataObserver*)obs.observer)->onDataChange(m, event);
      obsList << obs.observer;
      }
    }
  }

void SEntity::informTreeObservers(int m, const SChange *event, SObservers &obsList)
  {
  SProfileFunction
  foreach(const ObserverStruct &obs, _observers)
    {
    if(obs.mode == ObserverStruct::Tree)
      {
      ((STreeObserver*)obs.observer)->onTreeChange(m, event);
      obsList << obs.observer;
      }
    }
  SEntity *parentEnt = parentEntity();
  if(parentEnt)
    {
    parentEnt->informTreeObservers(m, event, obsList);
    }
  }

void SEntity::informConnectionObservers(int m, const SChange *event, SObservers &obsList)
  {
  SProfileFunction
  foreach(const ObserverStruct &obs, _observers)
    {
    if(obs.mode == ObserverStruct::Connection)
      {
      ((SConnectionObserver*)obs.observer)->onConnectionChange(m, event);
      obsList << obs.observer;
      }
    }
  }

