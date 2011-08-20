#include "scshiftproperty.h"
#include "sproperty.h"
#include "scembeddedtypes.h"
#include "sdatabase.h"
#include "sbaseproperties.h"

ScShiftProperty::ScShiftProperty(QScriptEngine *eng) : ScWrappedClass<SProperty *>(eng)
  {
  addMemberProperty("input", input, QScriptValue::PropertyGetter|QScriptValue::PropertySetter);
  addMemberProperty("outputs", outputs, QScriptValue::PropertyGetter);
  addMemberProperty("firstOutput", firstOutput, QScriptValue::PropertyGetter);
  addMemberProperty("name", name, QScriptValue::PropertyGetter|QScriptValue::PropertySetter);
  addMemberFunction("value", value);
  addMemberFunction("setValue", setValue);
  }

ScShiftProperty::~ScShiftProperty()
  {
  }

void ScShiftProperty::initiate()
  {
  initiateGlobalValue<ScShiftProperty>("SProperty");
  }

QScriptValue ScShiftProperty::input(QScriptContext *ctx, QScriptEngine *eng)
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

QScriptValue ScShiftProperty::name(QScriptContext *ctx, QScriptEngine *eng)
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

QScriptValue ScShiftProperty::firstOutput(QScriptContext *ctx, QScriptEngine *eng)
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

QScriptValue ScShiftProperty::value(QScriptContext *ctx, QScriptEngine *)
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

    if(prop->inheritsFromType<IntProperty>())
      {
      return prop->uncheckedCastTo<IntProperty>()->value();
      }
    else if(prop->inheritsFromType<BoolProperty>())
      {
      return prop->uncheckedCastTo<BoolProperty>()->value();
      }
    else if(prop->inheritsFromType<UnsignedIntProperty>())
      {
      return prop->uncheckedCastTo<UnsignedIntProperty>()->value();
      }
    else if(prop->inheritsFromType<LongIntProperty>())
      {
      return (qsreal)prop->uncheckedCastTo<LongIntProperty>()->value();
      }
    else if(prop->inheritsFromType<LongUnsignedIntProperty>())
      {
      return (qsreal)prop->uncheckedCastTo<LongUnsignedIntProperty>()->value();
      }
    else if(prop->inheritsFromType<FloatProperty>())
      {
      return prop->uncheckedCastTo<FloatProperty>()->value();
      }
    else if(prop->inheritsFromType<DoubleProperty>())
      {
      return prop->uncheckedCastTo<DoubleProperty>()->value();
      }
    else if(prop->inheritsFromType<StringProperty>())
      {
      return prop->uncheckedCastTo<StringProperty>()->value();
      }

    xAssertFail();
    //SPropertyData data;
    //prop->database()->write(prop, data, SPropertyData::Ascii);
    return "";//QString::fromUtf8(data.value());
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

    if(prop->inheritsFromType<IntProperty>())
      {
      prop->uncheckedCastTo<IntProperty>()->assign(arg.toInt32());
      return QScriptValue();
      }
    else if(prop->inheritsFromType<BoolProperty>())
      {
      prop->uncheckedCastTo<BoolProperty>()->assign(arg.toBool());
      return QScriptValue();
      }
    else if(prop->inheritsFromType<UnsignedIntProperty>())
      {
      prop->uncheckedCastTo<UnsignedIntProperty>()->assign(arg.toUInt32());
      return QScriptValue();
      }
    else if(prop->inheritsFromType<LongIntProperty>())
      {
      prop->uncheckedCastTo<LongIntProperty>()->assign((xint64)arg.toNumber());
      return QScriptValue();
      }
    else if(prop->inheritsFromType<LongUnsignedIntProperty>())
      {
      prop->uncheckedCastTo<LongUnsignedIntProperty>()->assign((xuint64)arg.toNumber());
      return QScriptValue();
      }
    else if(prop->inheritsFromType<FloatProperty>())
      {
      prop->uncheckedCastTo<FloatProperty>()->assign(arg.toNumber());
      return QScriptValue();
      }
    else if(prop->inheritsFromType<DoubleProperty>())
      {
      prop->uncheckedCastTo<DoubleProperty>()->assign(arg.toNumber());
      return QScriptValue();
      }
    else if(prop->inheritsFromType<StringProperty>())
      {
      prop->uncheckedCastTo<StringProperty>()->assign(arg.toString());
      return QScriptValue();
      }

    xAssertFail();
    //SPropertyData data;
    //data.setValue(arg.toString().toUtf8());
    //prop->database()->read(prop, data, SPropertyData::Ascii);
    return QScriptValue();
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.value(...);");
  return QScriptValue();
  }
