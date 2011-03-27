#include "scshiftpropertycontainer.h"
#include "spropertycontainer.h"
#include "scembeddedtypes.h"

ScShiftPropertyContainer::ScShiftPropertyContainer(QScriptEngine *eng, const QString &parent) : ScShiftProperty(eng, parent)
  {
  addMemberProperty("length", size, QScriptValue::PropertyGetter);
  addMemberProperty("size", size, QScriptValue::PropertyGetter);
  }

ScShiftPropertyContainer::~ScShiftPropertyContainer()
  {
  }

void ScShiftPropertyContainer::initiate()
  {
  setBlankConstructor<ScShiftPropertyContainer>("SPropertyContainer");
  }

QScriptValue ScShiftPropertyContainer::size(QScriptContext *ctx, QScriptEngine *eng)
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

QScriptClass::QueryFlags ScShiftPropertyContainer::queryProperty(const QScriptValue &object, const QScriptString &name, QueryFlags flags, uint *id)
  {
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

QScriptValue ScShiftPropertyContainer::property(const QScriptValue &object, const QScriptString &name, uint id)
  {
  ScProfileFunction
  SPropertyContainer *prop = (*unpackValue(object))->uncheckedCastTo<SPropertyContainer>();
  if (!prop)
    {
    xAssertFail();
    return 0;
    }

  if(id != (uint)-1)
    {
    SProperty *c = prop->firstChild();
    while(c && id)
      {
      --id;
      c = c->nextSibling();
      }

    if(c)
      {
      return ScEmbeddedTypes::packValue(c);
      }
    }
  else
    {
    return ScEmbeddedTypes::packValue(prop->findChild(name));
    }
  return QScriptValue();
  }

void ScShiftPropertyContainer::setProperty(QScriptValue &, const QScriptString &, uint, const QScriptValue &)
  {
  // dont do anything, but dont let users overwrite our properties.
  }

QScriptValue::PropertyFlags ScShiftPropertyContainer::propertyFlags(const QScriptValue &, const QScriptString &, uint)
  {
  return QScriptValue::Undeletable;
  }
