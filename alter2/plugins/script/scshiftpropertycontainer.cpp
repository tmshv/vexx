#if 0
#include "scshiftpropertycontainer.h"
#include "spropertycontainer.h"
#include "scembeddedtypes.h"

ScShiftPropertyContainerBase::ScShiftPropertyContainerBase(QScriptEngine *eng) : ScShiftPropertyBase(eng)
  {
  }

ScShiftPropertyContainerBase::~ScShiftPropertyContainerBase()
  {
  }

QScriptClass::QueryFlags ScShiftPropertyContainerBase::queryProperty(const QScriptValue &object, const QScriptString &name, QueryFlags flags, uint *id)
  {
  QScriptClass::QueryFlags fOut = ScShiftPropertyBase::queryProperty(object, name, flags, id);
  if(fOut)
    {
    *id = (uint)-2;
    return fOut;
    }

  ScProfileFunction
  SPropertyContainer *prop = (*unpackValue(object))->uncheckedCastTo<SPropertyContainer>();
  if (!prop)
    {
    xAssertFail();
    return 0;
    }

  *id = (uint)-1;
  bool isArrayIndex = false;
  qint32 pos = name.toArrayIndex(&isArrayIndex);
  if(isArrayIndex)
    {
    *id = pos;
    return flags;
    }
  else
    {
    SProperty *child = prop->findChild(name);
    if(child)
      {
      return flags;
      }
    }
  return 0;
  }

QScriptValue ScShiftPropertyContainerBase::property(const QScriptValue &object, const QScriptString &name, uint id)
  {
  if(id == (uint)-2)
    {
    return ScShiftPropertyBase::property(object, name, 0);
    }

  ScProfileFunction
  SPropertyContainer *prop = (*unpackValue(object))->uncheckedCastTo<SPropertyContainer>();
  if (!prop)
    {
    xAssertFail();
    return 0;
    }

  if(id != (uint)-1)
    {
    SProperty *c = prop->at(id);
    return ScEmbeddedTypes::packValue(c);
    }
  else
    {
    return ScEmbeddedTypes::packValue(prop->findChild(name));
    }
  return QScriptValue();
  }

void ScShiftPropertyContainerBase::setProperty(QScriptValue &v, const QScriptString &s, uint id, const QScriptValue &val)
  {
  if(id == (uint)-1)
    {
    ScShiftPropertyBase::setProperty(v, s, id, val);
    return;
    }
  // dont do anything, but dont let users overwrite our properties.
  }

QScriptValue::PropertyFlags ScShiftPropertyContainerBase::propertyFlags(const QScriptValue &v, const QScriptString &s, uint id)
  {
  return ScShiftPropertyBase::propertyFlags(v, s, id);
  }


ScShiftPropertyContainer::ScShiftPropertyContainer(QScriptEngine *eng) : ScShiftPropertyContainerBase(eng)
  {
  addMemberProperty("length", size, QScriptValue::PropertyGetter);
  addMemberProperty("size", size, QScriptValue::PropertyGetter);
  initiateGlobalValue<ScShiftPropertyContainer>("SPropertyContainer", "SProperty");
  }

ScShiftPropertyContainer::~ScShiftPropertyContainer()
  {
  }

QScriptValue ScShiftPropertyContainer::size(QScriptContext *ctx, QScriptEngine *)
  {
  ScProfileFunction
  SProperty **propPtr = getThis(ctx);
  if(propPtr)
    {
    SPropertyContainer *prop = (*propPtr)->uncheckedCastTo<SPropertyContainer>();
    if(prop)
      {
      return QScriptValue((quint32)prop->size());
      }
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SPropertyContainer.size(...);");
  return QScriptValue();
  }
#endif
