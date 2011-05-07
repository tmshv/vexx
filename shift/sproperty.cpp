#include "sproperty.h"
#include "sentity.h"
#include "sdatabase.h"
#include "schange.h"
#include "QString"
#include "sprocessmanager.h"
#include "XProfiler"

inline void setDependantsDirty(SProperty* prop, bool force)
  {
  for(SProperty *o=prop->output(); o; o = o->nextOutput())
    {
    o->setDirty(force);
    }

  const SPropertyInstanceInformation *child = prop->baseInstanceInformation();

  if(child && child->affects())
    {
    xsize i=0;
    while(child->affects()[i])
      {
      const SProperty SPropertyContainer::* affectsPtr(child->affects()[i]);
      SProperty *affectsProp = (SProperty*)&(prop->parent()->*affectsPtr);

      xAssert(affectsProp);
      affectsProp->setDirty(force);
      i++;
      }
    }

  // if we know the parent has an output
  if(prop->_flags.hasFlag(SProperty::ParentHasOutput))
    {
    SProperty *parent = prop;
    while(parent->_flags.hasFlag(SProperty::ParentHasOutput))
      {
      parent = parent->parent();
      parent->setDirty(force);
      }
    }
  }

bool SProperty::NameChange::apply(int mode, SObservers& obs)
  {
  SProfileFunction
  if(mode&Forward)
    {
    property()->internalSetName(after());
    }
  else if(mode&Backward)
    {
    property()->internalSetName(before());
    }
  if(mode&Inform)
    {
    xAssert(property()->entity());
    obs.clear();
    property()->entity()->informTreeObservers(mode, this, obs);
    }
  return true;
  }

SProperty::SProperty() : _nextSibling(0), _input(0), _output(0), _nextOutput(0),
    _database(0), _parent(0), _info(0), _instanceInfo(0), _entity(0), _flags(Dirty)
  {
  }

SProperty::~SProperty()
  {
  if(isDynamic())
    {
    ((InstanceInformation*)instanceInformation())->~InstanceInformation();
    }
  }

bool SProperty::isDynamic() const
  {
  SProfileFunction
  return instanceInformation()->dynamic();
  }

xsize SProperty::index() const
  {
  SProfileFunction
  preGet();
  return _info->propertyOffset() + _instanceInfo->index();
  }

void SProperty::setName(const QString &in)
  {
  SProfileFunction
  xAssert(isDynamic());
  xAssert(parent());

  QString fixedName = in;
  if(fixedName == "")
    {
    fixedName = typeInformation()->typeName();
    }

  if(name() == fixedName)
    {
    return;
    }

  // ensure the name is unique
  xsize num = 1;
  QString realName = fixedName;
  while(parent()->findChild(realName))
    {
    realName = fixedName + QString::number(num++);
    }

  void *changeMemory = getChange< NameChange >();
  NameChange *change = new(changeMemory) NameChange(name(), realName, this);
  database()->submitChange(change);
  }

SProperty *SProperty::nextSibling() const
  {
  if(parent())
    {
    parent()->preGet();
    }
  return _nextSibling;
  }

void SProperty::blankAssign(const SProperty *, SProperty *)
  {
  }

void SProperty::save(const SProperty *p, SPropertyData &d, SPropertyData::Mode X_UNUSED(m))
  {
  if(p->input())
    {
    d.appendAttribute("input", p->input()->path(p).toUtf8());
    }
  }

void SProperty::load(SProperty *p, const SPropertyData &d, xuint32, SPropertyData::Mode X_UNUSED(m), SLoader &loader)
  {
  if(d.hasAttribute("input"))
    {
    QString path();
    loader.resolveInputAfterLoad(p, QString::fromUtf8(d.attribute("input")));
    }
  }

bool SProperty::inheritsFromType(SPropertyType type) const
  {
  SProfileFunction
  const SPropertyInformation *current = typeInformation();
  xAssert(current);
  while(current)
    {
    if(current->typeId() == type)
      {
      return true;
      }
    current = current->parentTypeInformation();
    }
  return false;
  }

bool SProperty::inheritsFromType(const QString &type) const
  {
  SProfileFunction
  const SPropertyInformation *current = typeInformation();
  while(current)
    {
    if(current->typeName() == type)
      {
      return true;
      }
    current = current->parentTypeInformation();
    }
  return false;
  }

const QString &SProperty::name() const
  {
  SProfileFunction
  return baseInstanceInformation()->name();
  }

void SProperty::assign(const SProperty *propToAssign)
  {
  const SPropertyInformation *info = typeInformation();
  xAssert(info);

  info->assign()(propToAssign, this);
  }

SEntity *SProperty::entity() const
  {
  SProfileFunction
  if(_entity)
    {
    return _entity;
    }
  _entity = (SEntity *)castTo<SEntity>();
  if(_entity)
    {
    return _entity;
    }
  if(_parent)
    {
    _entity = _parent->entity();
    }
  return _entity;
  }

void *SProperty::getChangeMemory(size_t size) const
  {
  return parent()->database()->allocateChangeMemory(size);
  }

SPropertyType SProperty::type() const
  {
  return typeInformation()->typeId();
  }

void SProperty::connect(SProperty *prop) const
  {
  SProfileFunction
  if(prop && prop != this)
    {
    SProfileFunction
    void *changeMemory = getChange< ConnectionChange >();
    ConnectionChange *change = new(changeMemory) ConnectionChange(ConnectionChange::Connect, (SProperty*)this, prop);
    ((SDatabase*)database())->submitChange(change);
    }
  else
    {
    xAssertFail();
    }
  }

void SProperty::disconnect(SProperty *prop) const
  {
  SProfileFunction
  void *changeMemory = getChange< ConnectionChange >();
  ConnectionChange *change = new(changeMemory) ConnectionChange(ConnectionChange::Disconnect, (SProperty*)this, prop);
  ((SDatabase*)database())->submitChange(change);
  }

void SProperty::disconnect() const
  {
  SProfileFunction
  if(_input)
    {
    ((SProperty*)_input)->disconnect((SProperty*)this);
    }

  while(_output)
    {
    disconnect(_output);
    }
  }

bool SProperty::ConnectionChange::apply(int mode, SObservers &obs)
  {
  SProfileFunction
  if(mode&Forward)
    {
    if(_mode == Connect)
      {
      _driver->connectInternal(_driven);
      setParentHasInputConnection(_driven);
      setParentHasOutputConnection(_driver);
      setDependantsDirty(_driver, true);
      }
    else if(_mode == Disconnect)
      {
      _driver->disconnectInternal(_driven);
      clearParentHasInputConnection(_driven);
      clearParentHasOutputConnection(_driver);
      }
    }
  else if(mode&Backward)
    {
    if(_mode == Connect)
      {
      _driver->disconnectInternal(_driven);
      clearParentHasInputConnection(_driven);
      clearParentHasOutputConnection(_driver);
      }
    else if(_mode == Disconnect)
      {
      _driver->connectInternal(_driven);
      setParentHasInputConnection(_driven);
      setParentHasOutputConnection(_driver);
      setDependantsDirty(_driver, true);
      }
    }
  if(mode&Inform)
    {
    xAssert(_driver->entity());
    xAssert(_driven->entity());
    obs.clear();
    _driver->entity()->informConnectionObservers(mode, this, obs);
    _driven->entity()->informConnectionObservers(mode, this, obs);
    }
  return true;
  }

void SProperty::ConnectionChange::setParentHasInputConnection(SProperty *prop)
  {
  xAssert(prop);
  SPropertyContainer *cont = prop->castTo<SPropertyContainer>();
  if(cont)
    {
    SProperty *child = cont->firstChild();
    while(child)
      {
      if(!child->_flags.hasFlag(SProperty::ParentHasInput))
        {
        child->_flags.setFlag(SProperty::ParentHasInput);
        setParentHasInputConnection(child);
        }
      child = child->nextSibling();
      }
    }
  }

void SProperty::ConnectionChange::setParentHasOutputConnection(SProperty *prop)
  {
  xAssert(prop);
  SPropertyContainer *cont = prop->castTo<SPropertyContainer>();
  if(cont)
    {
    SProperty *child = cont->firstChild();
    while(child)
      {
      if(!child->_flags.hasFlag(SProperty::ParentHasOutput))
        {
        child->_flags.setFlag(SProperty::ParentHasOutput);
        setParentHasOutputConnection(child);
        }
      child = child->nextSibling();
      }
    }
  }

void SProperty::ConnectionChange::clearParentHasInputConnection(SProperty *prop)
  {
  xAssert(prop);
  SPropertyContainer *cont = prop->castTo<SPropertyContainer>();
  if(cont)
    {
    SProperty *child = cont->firstChild();
    while(child)
      {
      if(child->_flags.hasFlag(SProperty::ParentHasInput))
        {
        child->_flags.clearFlag(SProperty::ParentHasInput);
        clearParentHasInputConnection(child);
        }
      child = child->nextSibling();
      }
    }
  }

void SProperty::ConnectionChange::clearParentHasOutputConnection(SProperty *prop)
  {
  xAssert(prop);
  SPropertyContainer *cont = prop->castTo<SPropertyContainer>();
  if(cont)
    {
    SProperty *child = cont->firstChild();
    while(child)
      {
      if(child->_flags.hasFlag(SProperty::ParentHasOutput))
        {
        child->_flags.clearFlag(SProperty::ParentHasOutput);
        clearParentHasOutputConnection(child);
        }
      child = child->nextSibling();
      }
    }
  }

void SProperty::connectInternal(SProperty *prop) const
  {
  // prop can't already have an output
  if(prop->hasInput())
    {
    return;
    }
  prop->_input = (SProperty*)this;

  SProperty **output = (SProperty**)&_output;
  while(*output)
    {
    output = &((*output)->_nextOutput);
    }

  if(output)
    {
    *output = prop;
    }
  }

void SProperty::disconnectInternal(SProperty *prop) const
  {
  xAssert(prop->_input == this);

  prop->_input = 0;

  SProperty **output = (SProperty**)&_output;
  while(*output)
    {
    if((*output) == prop)
      {
      (*output) = (*output)->_nextOutput;
      return;
      }
    else
      {
      output = &((*output)->_nextOutput);
      }
    }
  }

QString SProperty::path() const
  {
  SProfileFunction
  if(parent() == 0)
    {
    return SDatabase::pathSeparator() + name();
    }
  return parent()->path() + SDatabase::pathSeparator() + name();
  }

QString SProperty::path(const SProperty *from) const
  {
  SProfileFunction
  if(from == this)
    {
    return "";
    }
  if(isDescendedFrom(from))
    {
    QString ret;
    const SProperty *p = parent();
    while(p && p != from)
      {
      xAssert(p->name() != "");
      ret = p->name() + SDatabase::pathSeparator() + ret;

      p = p->parent();
      }
    return ret + name();
    }
  else if(from->parent())
    {
    return ".." + SDatabase::pathSeparator() + path(from->parent());
    }
  xAssert(0);
  return "";
  }

bool SProperty::isDescendedFrom(const SProperty *in) const
  {
  SProfileFunction
  if(this == in)
    {
    return true;
    }
  else if(parent() == 0)
    {
    return false;
    }
  return parent()->isDescendedFrom(in);
  }

SProperty *SProperty::resolvePath(const QString &path)
  {
  SProfileFunction
  preGet();
  QStringList splitPath(path.split(SDatabase::pathSeparator()));
  SProperty *cur = this;
  foreach(const QString &name, splitPath)
    {
    if(!cur)
      {
      return 0;
      }
    if(name == "..")
      {
      cur = cur->parent();
      }

    SPropertyContainer* container = cur->castTo<SPropertyContainer>();
    if(!container)
      {
      return 0;
      }
    SProperty *child = container->firstChild();
    while(child)
      {
      if(child->name() == name)
        {
        cur = child;
        break;
        }
      child = child->nextSibling();
      }
    }
  return cur;
  }

const SProperty *SProperty::resolvePath(const QString &path) const
  {
  SProfileFunction
  preGet();
  QStringList splitPath(path.split(SDatabase::pathSeparator()));
  const SProperty *cur = this;
  foreach(const QString &name, splitPath)
    {
    if(!cur)
      {
      return 0;
      }
    if(name == "..")
      {
      cur = cur->parent();
      }

    const SPropertyContainer* container = cur->castTo<SPropertyContainer>();
    if(!container)
      {
      return 0;
      }
    const SProperty *child = container->firstChild();
    while(child)
      {
      if(child->name() == name)
        {
        cur = child;
        break;
        }
      child = child->nextSibling();
      }
    }
  return cur;
  }

void SProperty::internalSetName(const QString &name)
  {
  ((InstanceInformation*)this->baseInstanceInformation())->_name = name;
  }

void SProperty::postSet()
  {
  SProfileFunction
  _flags.clearFlag(Dirty);

  setDependantsDirty(this);
  }

void SProperty::setDirty(bool force)
  {
  if(!_flags.hasFlag(Dirty) || force)
    {
    _flags.setFlag(Dirty);

    setDependantsDirty(this, force);
    }
  }

void SProperty::preGet() const
  {
  SProfileFunction
  if(_flags.hasFlag(Dirty))
    {
    // this is a const function, but because we delay computation we may need to assign here
    SProperty *prop = const_cast<SProperty*>(this);
    prop->_flags.clearFlag(Dirty);
    prop->_flags.setFlag(PreGetting);

    const SPropertyInstanceInformation *child = baseInstanceInformation();
    if(child && child->compute())
      {
      xAssert(parent());
      SProcessManager::preCompute(child, parent());
      child->compute()(child, parent());
      }
    else if(input())
      {
      prop->assign(input());
      xAssert(!_flags.hasFlag(Dirty));
      }

    // dirty can be set again in compute functions.
    prop->_flags.clearFlag(Dirty);
    prop->_flags.clearFlag(PreGetting);

    xAssert(!_flags.hasFlag(Dirty));
    return;
    }

  if(_flags.hasFlag(ParentHasInput))
    {
    parent()->preGet();
    xAssert(!_flags.hasFlag(Dirty));
    }
  }
