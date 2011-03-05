#include "spropertycontainer.h"
#include "sdatabase.h"

S_PROPERTY_CONTAINER_EMPTY_DEFINITION(SPropertyContainer, SProperty, saveContainer, loadContainer)

SPropertyContainer::TreeChange::TreeChange(SPropertyContainer *b, SPropertyContainer *a, SProperty *ent, xsize index)
  : _before(b), _after(a), _property(ent), _index(index), _owner(false)
  {
  }

SPropertyContainer::TreeChange::~TreeChange()
  {
  if(_owner)
    {
    if(after())
      {
      after()->database()->deleteProperty(_property);
      }
    else if(before())
      {
      before()->database()->deleteProperty(_property);
      }
    else
      {
      xAssert(0 && "No parents?");
      }
    }
  }

bool SPropertyContainer::TreeChange::apply(int mode, SObservers &obs)
  {
  if(mode&Forward)
    {
    if(before())
      {
      _owner = true;
      before()->internalRemoveProperty(property());
      }
    if(after())
      {
      _owner = false;
      after()->internalInsertProperty(false, property(), _index);
      }
    }
  else if(mode&Backward)
    {
    if(after())
      {
      _owner = true;
      after()->internalRemoveProperty(property());
      }
    if(before())
      {
      _owner = false;
      before()->internalInsertProperty(false, property(), _index);
      }
    }
  if(mode&Inform)
    {
    xAssert(property()->entity());
    obs.clear();
    property()->entity()->informTreeObservers(mode, this, obs);
    }
  return true;
  }

SPropertyContainer::SPropertyContainer() : SProperty(), _child(0), _containedProperties(0)
  {
  }

xsize SPropertyContainer::size() const
  {
  preGet();
  xsize s = 0;
  SProperty *c = firstChild();
  while(c)
    {
    s++;
    c = c->nextSibling();
    }

  return s;
  }

const SProperty *SPropertyContainer::findChild(const QString &name) const
  {
  preGet();
  for(SProperty *child=firstChild(); child; child=child->nextSibling())
    {
    if(child->name() == name)
      {
      return child;
      }
    }
  return 0;
  }

SProperty *SPropertyContainer::findChild(const QString &name)
  {
  preGet();
  for(SProperty *child=firstChild(); child; child=child->nextSibling())
    {
    if(child->name() == name)
      {
      return child;
      }
    }
  return 0;
  }

bool SPropertyContainer::isChildDynamic(const SProperty *child) const
  {
  preGet();
  return indexOfChild(child) >= _containedProperties;
  }

xsize SPropertyContainer::indexOfChild(const SProperty *child) const
  {
  preGet();
  xsize propIndex = 0;
  SProperty *prop = _child;
  while(prop)
    {
    if(child == prop)
      {
      return propIndex;
      }
    propIndex++;
    prop = prop->_nextSibling;
    }
  return X_SIZE_SENTINEL;
  }

bool SPropertyContainer::contains(SProperty *child) const
  {
  preGet();
  SProperty *prop = _child;
  while(prop)
    {
    if(child == prop)
      {
      return true;
      }
    prop = prop->_nextSibling;
    }
  return false;
  }

SPropertyContainer::~SPropertyContainer()
  {
  xsize propIndex = 0;
  SProperty *prop = _child;
  while(prop)
    {
    SProperty *next = prop->_nextSibling;
    if(propIndex >= _containedProperties)
      {
      database()->deleteProperty(prop);
      }
    propIndex++;
    prop = next;
    }
  _child = 0;
  }

SProperty *SPropertyContainer::addProperty(xuint32 type, xsize index)
  {
  xAssert(index >= _containedProperties);

  SProperty *newProp = database()->createProperty(type);

  void *changeMemory = getChange< TreeChange >();
  TreeChange *change = new(changeMemory) TreeChange(0, this, newProp, index);
  database()->submitChange(change);
  postSet();
  return newProp;
  }

void SPropertyContainer::moveProperty(SPropertyContainer *c, SProperty *p)
  {
  xAssert(p->parent() == this);

  void *changeMemory = getChange< TreeChange >();
  TreeChange *change = new(changeMemory) TreeChange(this, c, p, indexOfChild(p));
  database()->submitChange(change);
  postSet();
  }

void SPropertyContainer::removeProperty(SProperty *oldProp)
  {
  xAssert(oldProp->parent() == this);

  SDatabase* db = database();
  xAssert(db);

  SBlock b(db);

  oldProp->disconnect();
  void *changeMemory = getChange< TreeChange >();
  TreeChange *change = new(changeMemory) TreeChange(this, 0, oldProp, indexOfChild(oldProp));
  database()->submitChange(change);
  postSet();
  }

void SPropertyContainer::assignContainer(const SProperty *f, SProperty *t)
  {
  if(t->type() == f->type())
    {
    const SPropertyContainer *from = f->uncheckedCastTo<SPropertyContainer>();
    SPropertyContainer *to = t->uncheckedCastTo<SPropertyContainer>();

    const SProperty *fChild=from->firstChild();
    SProperty *tChild=to->firstChild();
    xsize index = 0;
    xAssert(0 && "Test this fool... break and step through it.");
    while(fChild)
      {
      if(!tChild || tChild->type() != fChild->type())
        {
        xAssert(tChild->isDynamic());
        if(tChild)
          {
          to->removeProperty(tChild);
          }

        tChild = to->addProperty(fChild->type(), index);
        }

      tChild->assign(fChild);

      tChild = tChild->nextSibling();
      fChild = fChild->nextSibling();
      index++;
      }
    }
  }

void SPropertyContainer::saveContainer(const SProperty *p, SPropertyData &d, SPropertyData::Mode m)
  {
  const SPropertyContainer *c = p->castTo<SPropertyContainer>();
  xAssert(c);

  if(!c)
    {
    return;
    }

  const SDatabase *db = c->database();
  xAssert(db);

  if(!db)
    {
    return;
    }

  SProperty::save(p, d, m);
  for(const SProperty *child=c->firstChild(); child; child=child->nextSibling())
    {
    SPropertyData cData;

    db->write(child, cData, m);

    d.append(cData);
    }
  }

void SPropertyContainer::loadContainer(SProperty *p, const SPropertyData &d, xuint32 v, SPropertyData::Mode m, SLoader &l)
  {
  SPropertyContainer *c = p->castTo<SPropertyContainer>();
  xAssert(c);

  if(!c)
    {
    return;
    }

  SDatabase *db = c->database();
  xAssert(db);

  if(!db)
    {
    return;
    }

  SProperty::load(p, d, v, m, l);
  foreach(const SPropertyData &cData, d.childData())
    {
    db->read(cData, c, m);
    }
  }


void SPropertyContainer::internalInsertProperty(bool contained, SProperty *newProp, xsize index)
  {
  xAssert(newProp->_parent == 0);
  xAssert(newProp->_entity == 0);
  xAssert(newProp->_nextSibling == 0);

  if(_child)
    {
    xsize propIndex = 0;
    SProperty *prop = _child;
    while(prop)
      {
      if(index == (propIndex+1) || !prop->_nextSibling)
        {
        if(contained)
          {
          xAssert(_containedProperties == (propIndex+1));
          _containedProperties++;
          }
        // insert this prop into the list
        newProp->_nextSibling = prop->_nextSibling;
        prop->_nextSibling = newProp;

        // set up state info
        newProp->_parent = this;
        newProp->_entity = 0;
        newProp->_database = _database;
        break;
        }
      propIndex++;
      prop = prop->_nextSibling;
      }
    }
  else
    {
    if(contained)
      {
      xAssert(_containedProperties == 0);
      _containedProperties++;
      }
    _child = newProp;
    newProp->_parent = this;
    newProp->_entity = 0;
    newProp->_database = _database;
    }
  }

void SPropertyContainer::internalRemoveProperty(SProperty *oldProp)
  {
  xAssert(oldProp->parent() == this);

  if(oldProp == _child)
    {
    xAssert(_containedProperties == 0);

    _child = _child->_nextSibling;

    oldProp->_parent = this;
    oldProp->_entity = 0;
    oldProp->_database = 0;
    }
  else
    {
    xsize propIndex = 0;
    SProperty *prop = _child;
    while(prop)
      {
      if(oldProp == prop->_nextSibling)
        {
        xAssert(propIndex >= _containedProperties);

        oldProp->_parent = this;
        oldProp->_entity = 0;
        oldProp->_database = 0;

        prop->_nextSibling = oldProp->_nextSibling;
        }
      propIndex++;
      prop = prop->_nextSibling;
      }
    }
  }
