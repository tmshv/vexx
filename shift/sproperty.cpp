#include "sproperty.h"
#include "spropertyinformation.h"
#include "sinterface.h"
#include "sentity.h"
#include "sdatabase.h"
#include "schange.h"
#include "QString"
#include "sprocessmanager.h"
#include "XProfiler"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(SProperty)

SPropertyInformation *SProperty::createTypeInformation()
  {
  return SPropertyInformation::createNoParent<SProperty>("SProperty");
  }

inline void setDependantsDirty(SProperty* prop, bool force)
  {
  for(SProperty *o=prop->output(); o; o = o->nextOutput())
    {
    o->setDirty(force);
    }

  const SPropertyInstanceInformation *child = prop->baseInstanceInformation();

  if(child && child->affects())
    {
    SPropertyContainer *parent = prop->parent();
    const SPropertyInformation *parentInfo = parent->typeInformation();
    xsize i=0;
    while(child->affects()[i])
      {
      const SPropertyInstanceInformation *propInst = parentInfo->child(child->affects()[i]);
      SProperty *affectsProp = propInst->locateProperty(parent);

      xAssert(affectsProp);
      affectsProp->setDirty(force);
      i++;
      }
    }

  if(prop->input() || prop->isComputed() || prop->_flags.hasFlag(SProperty::ParentHasInput))
    {
    SPropertyContainer *c = prop->castTo<SPropertyContainer>();
    if(c)
      {
      SProperty *child = c->_child;
      while(child)
        {
        child->setDirty(force);
        child = child->_nextSibling;
        }
      }
    }

  // if we know the parent has an output
  if(prop->_flags.hasFlag(SProperty::ParentHasOutput))
    {
    SProperty *parent = prop;
    while(parent->_flags.hasFlag(SProperty::ParentHasOutput))
      {
      parent = parent->parent();
      // so we hit here when a child has been updated,
      // having a parent dirty when a child is up to date is a bit weird (possibly just wrong)
      // so we dirty the outputs of the parent, which do need to be updated.
      for(SProperty *o=parent->output(); o; o = o->nextOutput())
        {
        o->setDirty(force);
        }
      }
    }
  }

bool SProperty::NameChange::apply(int mode)
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
    property()->entity()->informTreeObservers(this);
    }
  return true;
  }

SProperty::SProperty() : _nextSibling(0), _input(0), _output(0), _nextOutput(0),
    _database(0), _parent(0), _info(0), _instanceInfo(0), _userData(0), _entity(0),
    _flags(Dirty)
  {
  }

SProperty::~SProperty()
  {
  UserData *ud = _userData;
  while(ud)
    {
    UserData *next = ud->_next;
    if(ud->onPropertyDelete(this))
      {
      delete ud;
      }
    ud = next;
    }

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
  if(parent())
    {
    parent()->preGet();
    }

  return _instanceInfo->index();
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

  database()->doChange<NameChange>(name(), realName, this);
  }

SProperty *SProperty::nextSibling() const
  {
  if(parent())
    {
    parent()->preGet();
    }
  return _nextSibling;
  }

void SProperty::assignProperty(const SProperty *, SProperty *)
  {
  }

void SProperty::saveProperty(const SProperty *p, SSaver &l)
  {
  const SPropertyInformation *type = p->typeInformation();

  l.setType(type);

  l.beginAttribute("name");
  writeValue(l, p->name());
  l.endAttribute("name");

  xuint32 v(type->version());
  if(v != 0)
    {
    l.beginAttribute("version");
    writeValue(l, v);
    l.endAttribute("version");
    }

  bool dyn(p->isDynamic());
  if(dyn)
    {
    l.beginAttribute("dynamic");
    writeValue(l, dyn ? 1 : 0);
    l.endAttribute("dynamic");
    }

  if(p->input())
    {
    l.beginAttribute("input");
    writeValue(l, p->input()->path(p));
    l.endAttribute("input");
    }
  }

SProperty *SProperty::loadProperty(SPropertyContainer *parent, SLoader &l)
  {
  const SPropertyInformation *type = l.type();
  xAssert(type);

  l.beginAttribute("name");
  QString name;
  readValue(l, name);
  l.endAttribute("name");

  l.beginAttribute("dynamic");
  xuint32 dynamic=false;
  readValue(l, dynamic);
  l.endAttribute("dynamic");

  l.beginAttribute("version");
  xuint32 version=0;
  readValue(l, version);
  l.endAttribute("version");

  SProperty *prop = 0;
  if(dynamic != 0)
    {
    prop = parent->database()->createDynamicProperty(type);
    xAssert(prop);

    parent->internalInsertProperty(false, prop, X_SIZE_SENTINEL);
    prop->setName(name);
    }
  else
    {
    prop = parent->findChild(name);
    xAssert(prop);
    xAssert(prop->typeInformation() == type);
    }

  l.beginAttribute("input");
  QString input;
  readValue(l, input);
  l.endAttribute("input");

  if(!input.isEmpty())
    {
    l.resolveInputAfterLoad(prop, input);
    }

  return prop;
  }

bool SProperty::inheritsFromType(const SPropertyInformation *type) const
  {
  SProfileFunction
  return typeInformation()->inheritsFromType(type);
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

void SProperty::connect(SProperty *prop) const
  {
  SProfileFunction
  if(prop && prop != this)
    {
    ((SDatabase*)database())->doChange<ConnectionChange>(ConnectionChange::Connect, (SProperty*)this, prop);
    }
  else
    {
    xAssertFail();
    }
  }

void SProperty::disconnect(SProperty *prop) const
  {
  SProfileFunction
  ((SDatabase*)database())->doChange<ConnectionChange>(ConnectionChange::Disconnect, (SProperty*)this, prop);
  }

bool SProperty::isComputed() const
  {
  return baseInstanceInformation()->compute() != 0;
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

bool SProperty::ConnectionChange::apply(int mode)
  {
  SProfileFunction
  if(mode&Forward)
    {
    if(_mode == Connect)
      {
      _driver->connectInternal(_driven);
      //if(_driven->typeInformation()->inheritsFromType(_driver->typeInformation()))
        {
        setParentHasInputConnection(_driven);
        setParentHasOutputConnection(_driver);
        }
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
      //if(_driven->typeInformation()->inheritsFromType(_driver->typeInformation()))
        {
        setParentHasInputConnection(_driven);
        setParentHasOutputConnection(_driver);
        }
      setDependantsDirty(_driver, true);
      }
    }

  if(mode&Inform)
    {
    if(_driver->entity())
      {
      _driver->entity()->informConnectionObservers(this);
      }
    if(_driven->entity())
      {
      _driven->entity()->informConnectionObservers(this);
      }
    }
  return true;
  }

void SProperty::ConnectionChange::setParentHasInputConnection(SProperty *prop)
  {
  xAssert(prop);
  prop->_flags.setFlag(ParentHasInput);

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
      if(child->_flags.hasFlag(SProperty::ParentHasInput) &&
         (!prop->parent()->input() &&
          !prop->parent()->instanceInformation()->isComputed() &&
          !prop->parent()->_flags.hasFlag(ParentHasInput)))
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
      if(child->_flags.hasFlag(SProperty::ParentHasOutput) &&
         (!prop->parent()->output() &&
          !prop->parent()->instanceInformation()->affectsSiblings() &&
          !prop->parent()->_flags.hasFlag(ParentHasOutput)))
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
  if((!_flags.hasAnyFlags(Dirty|PreGetting)) || force)
    {
    _flags.setFlag(Dirty);

    setDependantsDirty(this, force);

    if(entity())
      {
      entity()->informDirtyObservers(this);
      }
    }
  }

void SProperty::update() const
  {
  SProfileFunction
  bool stateStorageEnabled = database()->stateStorageEnabled();
  const_cast<SDatabase*>(database())->setStateStorageEnabled(false);

  SProperty *prop = const_cast<SProperty*>(this);
  prop->_flags.setFlag(PreGetting);

  if(_flags.hasFlag(ParentHasInput))
    {
    parent()->preGet();
    }

  // this is a const function, but because we delay computation we may need to assign here
  prop->_flags.clearFlag(Dirty);

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
    }

  // dirty can be set again in compute functions.
  prop->_flags.clearFlag(Dirty);
  prop->_flags.clearFlag(PreGetting);

  const_cast<SDatabase*>(database())->setStateStorageEnabled(stateStorageEnabled);

  xAssert(!_flags.hasFlag(Dirty));
  }

void SProperty::addUserData(UserData *userData)
  {
  xAssert(userData);
  xAssert(!userData->next());
  if(userData && !userData->next())
    {
    userData->_next = _userData;
    _userData = userData;
    }
  }

void SProperty::removeUserData(UserData *userData)
  {
  xAssert(userData);
  xAssert(userData->_next);
  UserData *last = 0;
  UserData *ud = _userData;
  while(ud)
    {
    if(ud == userData)
      {
      if(last)
        {
        last->_next = userData->_next;
        }
      else
        {
        _userData = userData->_next;
        }
      userData->_next = 0;
      break;
      }
    last = ud;
    ud = ud->next();
    }
  xAssert(!userData->_next);
  }

SInterfaceBase *SProperty::interface(xuint32 typeId)
  {
  const SInterfaceBaseFactory* factory = typeInformation()->interfaceFactory(typeId);
  if(factory)
    {
    return const_cast<SInterfaceBaseFactory*>(factory)->classInterface(this);
    }
  return 0;
  }

const SInterfaceBase *SProperty::interface(xuint32 typeId) const
  {
  const SInterfaceBaseFactory* factory = typeInformation()->interfaceFactory(typeId);
  if(factory)
    {
    return const_cast<SInterfaceBaseFactory*>(factory)->classInterface(const_cast<SProperty*>(this));
    }
  return 0;

  }
