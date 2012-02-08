#include "scshiftproperty.h"
#include "sproperty.h"
#include "scembeddedtypes.h"
#include "sdatabase.h"
#include "sbaseproperties.h"
#include "sadocument.h"
#include "scplugin.h"

ScShiftPropertyBase::ScShiftPropertyBase(QScriptEngine *eng) : ScWrappedClass<SProperty *>(eng)
  {
  }

QScriptClass::QueryFlags ScShiftPropertyBase::queryProperty(const QScriptValue &object, const QScriptString &name, QueryFlags flags, uint *)
  {
  ScProfileFunction
  SProperty *prop = (*unpackValue(object))->uncheckedCastTo<SProperty>();
  if (!prop)
    {
    xAssertFail();
    return 0;
    }

  const SPropertyInformation *i = prop->typeInformation();
  if(i->wrappedMembers().contains(name))
    {
    return flags;
    }
  return 0;
  }

QScriptValue ScShiftPropertyBase::memberFunction(QScriptContext *c, QScriptEngine *e)
  {
  QScriptValue thisObject = c->callee();
  xAssert(thisObject.isFunction());
  QScriptValue thisObjectData = thisObject.data();

  double thisPropDbl = thisObjectData.property("thisProp").toNumber();

  SProperty *thisProp = *reinterpret_cast<SProperty **>(&thisPropDbl);
  xAssert(thisProp);

  double memberDbl = thisObjectData.property("thisFunction").toNumber();
  const SPropertyInformation::WrappedMember *member = *reinterpret_cast<const SPropertyInformation::WrappedMember **>(&memberDbl);
  xAssert(member);

  if(member->expectedArguments != c->argumentCount())
    {
    c->throwError(QScriptContext::SyntaxError, "Incorrect argument count to SProperty." + member->memberName +  "(...), expected " + QString::number(member->expectedArguments));
    return QScriptValue();
    }
  QVariant wrappedArgs[SPropertyInformation::MaxWrappedArguments];

  for(xuint32 i = 0; i < member->expectedArguments; ++i)
    {
    wrappedArgs[i] = c->argument(i).toVariant();
    }

  bool success = true;
  QString errorString;
  QVariant ret = member->function(thisProp, wrappedArgs, member->expectedArguments, &success, &errorString);

  if(!success)
    {
    c->throwError(QScriptContext::SyntaxError, errorString + " in SProperty." + member->memberName +  "(...)");
    return QScriptValue();
    }

  if(ret.isNull())
    {
    return QScriptValue();
    }
  return e->newVariant(ret);
  }

QScriptValue ScShiftPropertyBase::property(const QScriptValue &object, const QScriptString &name, uint)
  {
  ScProfileFunction
  SProperty *prop = (*unpackValue(object))->uncheckedCastTo<SProperty>();
  if (!prop)
    {
    xAssertFail();
    return 0;
    }

  const SPropertyInformation *i = prop->typeInformation();
  xAssert(i->wrappedMembers().contains(name));

  const SPropertyInformation::WrappedMember *mem = &i->wrappedMembers().find(name).value();

  QScriptValue v = engine()->newFunction(memberFunction);

  QScriptValue data = engine()->newObject();
  double propDbl = *reinterpret_cast<double *>(&prop);
  double fnDbl = *reinterpret_cast<const double *>(&mem);

  data.setProperty("thisProp", propDbl);
  data.setProperty("thisFunction", fnDbl);

  v.setData(data);

  return v;
  }

void ScShiftPropertyBase::setProperty(QScriptValue &, const QScriptString &, uint, const QScriptValue &)
  {
  // dont do anything, but dont let users overwrite our properties.
  }

QScriptValue::PropertyFlags ScShiftPropertyBase::propertyFlags(const QScriptValue &, const QScriptString &, uint)
  {
  return QScriptValue::Undeletable;
  }


ScShiftProperty::ScShiftProperty(QScriptEngine *eng) : ScShiftPropertyBase(eng)
  {
  addMemberProperty("typeInformation", typeInformation, QScriptValue::PropertyGetter);
  addMemberProperty("input", input, QScriptValue::PropertyGetter|QScriptValue::PropertySetter);
  addMemberProperty("parent", parent, QScriptValue::PropertyGetter|QScriptValue::PropertySetter);
  addMemberProperty("outputs", outputs, QScriptValue::PropertyGetter);
  addMemberProperty("firstOutput", firstOutput, QScriptValue::PropertyGetter);
  addMemberProperty("affects", affects, QScriptValue::PropertyGetter);
  addMemberProperty("dynamic", dynamic, QScriptValue::PropertyGetter);
  addMemberProperty("name", name, QScriptValue::PropertyGetter|QScriptValue::PropertySetter);
  addMemberProperty("valueString", valueString, QScriptValue::PropertyGetter);
  addMemberFunction("value", value);
  addMemberFunction("setValue", setValue);
  addMemberFunction("pathTo", pathTo);

  addMemberFunction("beginBlock", beginBlock);
  addMemberFunction("endBlock", endBlock);

  addMemberFunction("registerExporter", registerExporter);

  initiateGlobalValue<ScShiftProperty>("SProperty");
  }

ScShiftProperty::~ScShiftProperty()
  {
  }

QScriptValue ScShiftProperty::pathTo(QScriptContext *ctx, QScriptEngine *)
  {
  ScProfileFunction
  SProperty **prop = getThis(ctx);
  if(prop && *prop)
    {
    if(ctx->argumentCount() < 1)
      {
      ctx->throwError(QScriptContext::SyntaxError, "Incorrect argument count to SProperty.pathTo(property);");
      return QScriptValue();
      }

    SProperty **toPtr = unpackValue(ctx->argument(0));
    if(toPtr)
      {
      return (*toPtr)->path(*prop);
      }

    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.pathTo(property);");
  return QScriptValue();
  }

QScriptValue ScShiftProperty::input(QScriptContext *ctx, QScriptEngine *)
  {
  ScProfileFunction
  SProperty **prop = getThis(ctx);
  if(prop && *prop)
    {
    if(ctx->argumentCount() == 1)
      {
      SProperty **newInput = unpackValue(ctx->argument(0));
      if(newInput)
        {
        (*newInput)->connect(*prop);
        }
      else
        {
        ctx->throwError(QScriptContext::SyntaxError, "Incorrect argument to SProperty.input(...); expected property type");
        return QScriptValue();
        }
      }
    return ScEmbeddedTypes::packValue((*prop)->input());
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.input(...);");
  return QScriptValue();
  }

QScriptValue ScShiftProperty::parent(QScriptContext *ctx, QScriptEngine *)
  {
  ScProfileFunction
  SProperty **prop = getThis(ctx);
  if(prop && *prop)
    {
    if(ctx->argumentCount() == 1)
      {
      SProperty **newParent = unpackValue(ctx->argument(0));
      if(newParent && *newParent)
        {
        SPropertyContainer *cont = (*newParent)->castTo<SPropertyContainer>();
        if(cont)
          {
          (*prop)->parent()->moveProperty(cont, *prop);
          }
        }
      else
        {
        ctx->throwError(QScriptContext::SyntaxError, "Incorrect argument to SProperty.input(...); expected property type");
        return QScriptValue();
        }
      }
    return ScEmbeddedTypes::packValue((*prop)->parent());
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.input(...);");
  return QScriptValue();
  }

QScriptValue ScShiftProperty::name(QScriptContext *ctx, QScriptEngine *)
  {
  ScProfileFunction
  SProperty **prop = getThis(ctx);
  if(prop && *prop)
    {
    if(ctx->argumentCount() == 1)
      {
      (*prop)->setName(ctx->argument(1).toString());
      }
    return QScriptValue((*prop)->name());
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.name(...);");
  return QScriptValue();
  }

QScriptValue ScShiftProperty::outputs(QScriptContext *ctx, QScriptEngine *eng)
  {
  ScProfileFunction
  SProperty **prop = getThis(ctx);
  if(prop && *prop)
    {
    QScriptValue ret(eng->newArray());
    SProperty *output = (*prop)->output();
    xuint32 index = 0;
    while(output)
      {
      ret.setProperty(index, ScEmbeddedTypes::packValue(output));
      output = output->nextOutput();
      ++index;
      }
    return ret;
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.outputs(...);");
  return QScriptValue();
  }

QScriptValue ScShiftProperty::typeInformation(QScriptContext *ctx, QScriptEngine *eng)
  {
  ScProfileFunction
  SProperty **prop = getThis(ctx);
  if(prop && *prop)
    {
    const SPropertyInformation *info = (*prop)->typeInformation();
    QScriptValue value = eng->globalObject().property("dbTypes").property(info->typeName());

    if(!value.isObject())
      {
      ctx->throwError(QScriptContext::SyntaxError, "Could not find global object " + info->typeName() + " to SProperty.typeInformation(...);");
      return QScriptValue();
      }

    return value;
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.outputs(...);");
  return QScriptValue();
  }

QScriptValue ScShiftProperty::firstOutput(QScriptContext *ctx, QScriptEngine *)
  {
  ScProfileFunction
  SProperty **prop = getThis(ctx);
  if(prop && *prop)
    {
    return ScEmbeddedTypes::packValue((*prop)->output());
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.firstOutput(...);");
  return QScriptValue();
  }

QScriptValue ScShiftProperty::affects(QScriptContext *ctx, QScriptEngine *e)
  {
  ScProfileFunction
  SProperty **prop = getThis(ctx);
  if(prop && *prop)
    {
    const SPropertyInstanceInformation *info = (*prop)->instanceInformation();
    xsize *affects = info->affects();
    if(affects && affects[0] != 0)
      {
      QScriptValue ret = e->newArray();
      xsize i = 0;
      const SPropertyInformation *parentInfo = (*prop)->parent()->typeInformation();
      while(*affects)
        {
        const SPropertyInstanceInformation *affected = parentInfo->child(*affects);
        QString affectedName = affected->name();

        ret.setProperty(i++, affectedName);
        affects++;
        }

      return ret;
      }

    return QScriptValue();
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.firstOutput(...);");
  return QScriptValue();
  }


QScriptValue ScShiftProperty::dynamic(QScriptContext *ctx, QScriptEngine *)
  {
  ScProfileFunction
  SProperty **prop = getThis(ctx);
  if(prop && *prop)
    {
    return (*prop)->isDynamic();
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.firstOutput(...);");
  return QScriptValue();
  }

QScriptValue toScriptValue(QScriptEngine *e, const QVariant &v)
  {
  if(v.isNull() || !v.isValid())
    {
    return QScriptValue();
    }
  else if(v.type() == QVariant::Bool)
    {
    return v.value<bool>();
    }
  else if(v.type() == QVariant::String)
    {
    return v.value<QString>();
    }
  else if(v.canConvert<double>())
    {
    return v.value<double>();
    }

  // arrays, objects?
  return e->newVariant(v);
  }

QScriptValue ScShiftProperty::value(QScriptContext *ctx, QScriptEngine *e)
  {
  ScProfileFunction
  SProperty **propPtr = getThis(ctx);
  if(propPtr && *propPtr)
    {
    SProperty *prop = *propPtr;
    if(prop->inheritsFromType<SPropertyContainer>())
      {
      ctx->throwError(QScriptContext::SyntaxError, "Can't pack the value for type " + prop->typeInformation()->typeName() + " in SProperty.value(...);");
      return QScriptValue();
      }

    SPropertyVariantInterface *varInt = prop->interface<SPropertyVariantInterface>();

    if(varInt)
      {
      return toScriptValue(e, varInt->asVariant(prop));
      }
    else
      {
      ctx->throwError(QScriptContext::SyntaxError, "Unable to retrieve value from property.");
      }
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.value(...);");
  return QScriptValue();
  }

QScriptValue ScShiftProperty::valueString(QScriptContext *ctx, QScriptEngine *e)
  {
  ScProfileFunction
  SProperty **propPtr = getThis(ctx);
  if(propPtr && *propPtr)
    {
    SProperty *prop = *propPtr;
    if(prop->inheritsFromType<SPropertyContainer>())
      {
      return QScriptValue();
      }

    SPropertyVariantInterface *varInt = prop->interface<SPropertyVariantInterface>();

    if(varInt)
      {
      return toScriptValue(e, varInt->asString(prop));
      }
    else
      {
      return QScriptValue();
      }
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.value(...);");
  return QScriptValue();
  }

QScriptValue ScShiftProperty::setValue(QScriptContext *ctx, QScriptEngine *)
  {
  ScProfileFunction
  SProperty **propPtr = getThis(ctx);
  if(propPtr && *propPtr)
    {
    SProperty *prop = *propPtr;
    if(prop->inheritsFromType<SPropertyContainer>())
      {
      ctx->throwError(QScriptContext::SyntaxError, "Can't pack the value for type " + prop->typeInformation()->typeName() + " in SProperty.value(...);");
      return QScriptValue();
      }

    if(ctx->argumentCount() != 1)
      {
      ctx->throwError(QScriptContext::SyntaxError, "Can't pack the value for type " + prop->typeInformation()->typeName() + " in SProperty.value(...);");
      return QScriptValue();
      }

    QScriptValue arg = ctx->argument(0);

    SPropertyVariantInterface *varInt = prop->interface<SPropertyVariantInterface>();

    if(varInt)
      {
      varInt->setVariant(prop, arg.toVariant());
      return QScriptValue();
      }
    else
      {
      ctx->throwError(QScriptContext::SyntaxError, "Unable to assign value to property.");
      }
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.value(...);");
  return QScriptValue();
  }

QScriptValue ScShiftProperty::registerExporter(QScriptContext *ctx, QScriptEngine *)
  {
  QScriptValue caller = ctx->thisObject();
  QString str = caller.property("typeName").toString();

  const SPropertyInformation *info = STypeRegistry::findType(str);
  if(!info)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Cannot find type " + str + " to add exporter.");
    }

  if(ctx->argumentCount() != 1)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect first argument to SProperty.registerExporter(...), expected object");
    }

  QScriptValue exp = ctx->argument(0);

  class ScriptExporter : public SExportableInterface::Exporter
    {
  public:
    ScriptExporter(QScriptValue v) : _v(v)
      {
      }

    virtual bool exportFile(const QString &file, SProperty *prop) const
      {
      QScriptValue exFn(_v.property("exportFile"));

      if(!exFn.isFunction())
        {
        return false;
        }

      QScriptValueList l;
      l << file;
      l << ScEmbeddedTypes::packValue(prop);

      QScriptValue r = ScPlugin::call(exFn, _v, l);

      return r.toBool();
      }

    virtual QString exporterName() const
      {
      return _v.property("name").toString();
      }

    virtual QString exporterFileType() const
      {
      return _v.property("fileType").toString();
      }

  private:
    QScriptValue _v;
    };

  SExportableInterface::Exporter *exporter = new ScriptExporter(exp);
  SExportableInterface::addExporter(info, exporter);

  return QScriptValue();
  }

QScriptValue ScShiftProperty::beginBlock(QScriptContext *ctx, QScriptEngine *)
  {
  ScProfileFunction
  SProperty **propPtr = getThis(ctx);
  if(!propPtr || !*propPtr)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SDatabase.beginBlock(...);");
    return QScriptValue();
    }

  if(ctx->argumentCount() != 0)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SDatabase.beginBlock(...);");
    return QScriptValue();
    }

  (*propPtr)->handler()->beginBlock();
  return QScriptValue();
  }

QScriptValue ScShiftProperty::endBlock(QScriptContext *ctx, QScriptEngine *)
  {
  ScProfileFunction
  SProperty **propPtr = getThis(ctx);
  if(!propPtr || !*propPtr)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SDatabase.endBlock(...);");
    return QScriptValue();
    }

  if(ctx->argumentCount() > 1)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SDatabase.endBlock(...);");
    return QScriptValue();
    }

  bool cancel = false;
  if(ctx->argumentCount() == 1)
    {
    cancel = ctx->argument(0).toBoolean();
    }

  (*propPtr)->handler()->endBlock(cancel);
  return QScriptValue();
  }

