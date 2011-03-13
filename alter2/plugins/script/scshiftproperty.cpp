#include "scshiftproperty.h"
#include "sproperty.h"

#define EXPOSED_NAME "Property"

ScShiftProperty::ScShiftProperty(QScriptEngine *eng) : ScWrappedClass<SProperty *, ScShiftProperty>(eng, "Object")
  {
  addMemberProperty("input", input, QScriptValue::PropertyGetter);
  addMemberProperty("outputs", outputs, QScriptValue::PropertyGetter);
  addMemberProperty("firstOutput", firstOutput, QScriptValue::PropertyGetter);
  }

void ScShiftProperty::initiate()
  {
  setBlankConstructor(EXPOSED_NAME);
  }

QScriptValue ScShiftProperty::input(QScriptContext *ctx, QScriptEngine *eng)
  {
  SProperty **prop = getThis(ctx);
  xAssert(prop);
  if(*prop)
    {
    return packValue(eng, EXPOSED_NAME, (*prop)->input());
    }
  return QScriptValue();
  }

QScriptValue ScShiftProperty::outputs(QScriptContext *ctx, QScriptEngine *eng)
  {
  SProperty **prop = getThis(ctx);
  if(*prop)
    {
    QScriptValue ret(eng->newArray());
    SProperty *output = (*prop)->output();
    xuint32 index = 0;
    while(output)
      {
      ret.setProperty(index, packValue(eng, EXPOSED_NAME, output));
      output = output->nextOutput();
      ++index;
      }
    return ret;
    }
  return QScriptValue();
  }

QScriptValue ScShiftProperty::firstOutput(QScriptContext *ctx, QScriptEngine *eng)
  {
  SProperty **prop = getThis(ctx);
  if(*prop)
    {
    return packValue(eng, EXPOSED_NAME, (*prop)->output());
    }
  return QScriptValue();
  }
