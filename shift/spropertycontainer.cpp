#include "spropertycontainer.h"
#include "sdatabase.h"

S_PROPERTY_CONTAINER_EMPTY_DEFINITION(SPropertyContainer)

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
      after()->database()->deleteDynamicProperty(_property);
      }
    else if(before())
      {
      before()->database()->deleteDynamicProperty(_property);
      }
    else
      {
      xAssert(0 && "No parents?");
      }
    }
  }

bool SPropertyContainer::TreeChange::apply(int mode)
  {
  if(mode&Forward)
    {
    if(before())
      {
      _owner = true;
      before()->internalRemoveProperty(property());
      before()->postSet();
      }
    if(after())
      {
      _owner = false;
      after()->internalInsertProperty(false, property(), _index);
      after()->postSet();
      }
    }
  else if(mode&Backward)
    {
    if(after())
      {
      _owner = true;
      after()->internalRemoveProperty(property());
      after()->postSet();
      }
    if(before())
      {
      _owner = false;
      before()->internalInsertProperty(false, property(), _index);
      before()->postSet();
      }
    }
  if(mode&Inform)
    {
    xAssert(property()->entity());
    property()->entity()->informTreeObservers(this);
    if(before())
      {
      before()->entity()->informTreeObservers(this);
      }
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
  xAssert(database());

  xsize propIndex = 0;
  SProperty *prop = _child;
  while(prop)
    {
    SProperty *next = prop->_nextSibling;
    if(propIndex >= _containedProperties)
      {
      database()->deleteDynamicProperty(prop);
      }
    propIndex++;
    prop = next;
    }
  _child = 0;
  }

SProperty *SPropertyContainer::addProperty(xuint32 type, xsize index)
  {
  xAssert(index >= _containedProperties);

  SProperty *newProp = database()->createDynamicProperty(type);

  void *changeMemory = getChange< TreeChange >();
  TreeChange *change = new(changeMemory) TreeChange(0, this, newProp, index);
  database()->submitChange(change);
  return newProp;
  }

void SPropertyContainer::moveProperty(SPropertyContainer *c, SProperty *p)
  {
  xAssert(p->parent() == this);

  void *changeMemory = getChange< TreeChange >();
  TreeChange *change = new(changeMemory) TreeChange(this, c, p, p->index());
  database()->submitChange(change);
  }

void SPropertyContainer::removeProperty(SProperty *oldProp)
  {
  xAssert(oldProp->parent() == this);

  SDatabase* db = database();
  xAssert(db);

  SBlock b(db);

  oldProp->disconnect();
  void *changeMemory = getChange< TreeChange >();
  TreeChange *change = new(changeMemory) TreeChange(this, 0, oldProp, oldProp->index());
  database()->submitChange(change);
  }

void SPropertyContainer::assignProperty(const SProperty *f, SProperty *t)
  {
  const SPropertyContainer *from = f->uncheckedCastTo<SPropertyContainer>();
  SPropertyContainer *to = t->uncheckedCastTo<SPropertyContainer>();

  if(from->containedProperties() == to->containedProperties())
    {
    const SProperty *fChild=from->firstChild();
    SProperty *tChild=to->_child;
    xsize index = 0;
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

      fChild = fChild->nextSibling();
      tChild = tChild->_nextSibling;
      index++;
      }
    }
  else
    {
    xAssertFail();
    }
  }

void SPropertyContainer::saveProperty(const SProperty *p, SSaver &l)
  {
  const SPropertyContainer *c = p->uncheckedCastTo<SPropertyContainer>();
  xAssert(c);

  SProperty::saveProperty(p, l);

  l.setChildCount(c->size());
  for(const SProperty *child=c->firstChild(); child; child=child->nextSibling())
    {
    l.beginNextChild();

    l.write(child);

    l.endNextChild();
    }
  }

SProperty *SPropertyContainer::loadProperty(SPropertyContainer *parent, SLoader &l)
  {
  xAssert(parent);

  SProperty *prop = SProperty::loadProperty(parent, l);
  xAssert(prop);

  SPropertyContainer* container = prop->uncheckedCastTo<SPropertyContainer>();

  for(xsize i=0, s=l.childCount(); i<s; ++i)
    {
    l.beginNextChild();
    l.read(container);
    l.endNextChild();
    }

  return prop;
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
        else
          {
          ((SProperty::InstanceInformation*)newProp->_instanceInfo)->_index = propIndex;
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
    else
      {
      ((SProperty::InstanceInformation*)newProp->_instanceInfo)->_index = 0;
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
    ((SProperty::InstanceInformation*)oldProp->_instanceInfo)->_index = X_SIZE_SENTINEL;
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
        ((SProperty::InstanceInformation*)oldProp->_instanceInfo)->_index = X_SIZE_SENTINEL;

        prop->_nextSibling = oldProp->_nextSibling;
        }
      propIndex++;
      prop = prop->_nextSibling;
      }
    }
  }

const SProperty *SPropertyContainer::at(xsize i) const
  {
  const SProperty *c = firstChild();
  while(c && i)
    {
    --i;
    c = c->nextSibling();
    }

  return c;
  }

SProperty *SPropertyContainer::at(xsize i)
  {
  SProperty *c = firstChild();
  while(c && i)
    {
    --i;
    c = c->nextSibling();
    }

  return c;
  }
