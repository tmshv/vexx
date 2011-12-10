#include "scshiftproperty.h"
#include "sproperty.h"
#include "scembeddedtypes.h"
#include "sdatabase.h"
#include "sbaseproperties.h"
#include "sadocument.h"
#include "scplugin.h"

ScShiftProperty::ScShiftProperty(QScriptEngine *eng) : ScWrappedClass<SProperty *>(eng)
  {
  addMemberProperty("typeInformation", typeInformation, QScriptValue::PropertyGetter);
  addMemberProperty("input", input, QScriptValue::PropertyGetter|QScriptValue::PropertySetter);
  addMemberProperty("outputs", outputs, QScriptValue::PropertyGetter);
  addMemberProperty("firstOutput", firstOutput, QScriptValue::PropertyGetter);
  addMemberProperty("affects", affects, QScriptValue::PropertyGetter);
  addMemberProperty("dynamic", dynamic, QScriptValue::PropertyGetter);
  addMemberProperty("name", name, QScriptValue::PropertyGetter|QScriptValue::PropertySetter);
  addMemberProperty("valueString", valueString, QScriptValue::PropertyGetter);
  addMemberFunction("value", value);
  addMemberFunction("setValue", setValue);

  addMemberFunction("registerExporter", registerExporter);

  initiateGlobalValue<ScShiftProperty>("SProperty");
  }

ScShiftProperty::~ScShiftProperty()
  {
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
    QScriptValue value = eng->globalObject().property(info->typeName());

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


QScriptValue ScShiftProperty::dynamic(QScriptContext *ctx, QScriptEngine *e)
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
      ctx->throwError(QScriptContext::SyntaxError, "Can't pack the value for type " + prop->typeInformation()->typeName() + " in SProperty.value(...);");
      return QScriptValue();
      }

    SPropertyVariantInterface *varInt = prop->interface<SPropertyVariantInterface>();

    if(varInt)
      {
      return toScriptValue(e, varInt->asString(prop));
      }
    else
      {
      ctx->throwError(QScriptContext::SyntaxError, "Unable to retrieve value from property.");
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
