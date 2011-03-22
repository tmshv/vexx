#include "scshiftproperty.h"
#include "sproperty.h"
#include "scembeddedtypes.h"

ScShiftProperty::ScShiftProperty(QScriptEngine *eng, const QString &parent) : ScWrappedClass<SProperty *>(eng, parent)
  {
  addMemberProperty("input", input, QScriptValue::PropertyGetter);
  addMemberProperty("outputs", outputs, QScriptValue::PropertyGetter);
  addMemberProperty("firstOutput", firstOutput, QScriptValue::PropertyGetter);
  addMemberProperty("name", name, QScriptValue::PropertyGetter|QScriptValue::PropertySetter);
  }

ScShiftProperty::~ScShiftProperty()
  {
  }

void ScShiftProperty::initiate()
  {
  setBlankConstructor<ScShiftProperty>("SProperty");
  }

QScriptValue ScShiftProperty::input(QScriptContext *ctx, QScriptEngine *eng)
  {
  SProperty **prop = getThis(ctx);
  if(prop && *prop)
    {
    if(ctx->argumentCount() == 1)
      {
      SProperty **newInput = unpackValue(ctx->argument(1));
      (*newInput)->connect(*prop);
      }
    return ScEmbeddedTypes::packValue((*prop)->input());
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.input(...);");
  return QScriptValue();
  }

QScriptValue ScShiftProperty::name(QScriptContext *ctx, QScriptEngine *eng)
  {
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
  SProperty **prop = getThis(ctx);
  if(prop && *prop)
    {
    return ScEmbeddedTypes::packValue((*prop)->output());
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SProperty.firstOutput(...);");
  return QScriptValue();
  }
