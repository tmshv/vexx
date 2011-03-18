#include "scshiftdynamicpropertyinformation.h"
#include "sdynamicpropertyinformation.h"
#include "QScriptEngine"

#define EXPOSED_NAME "DynamicPropertyInformation"

ScShiftDynamicPropertyInformation::ScShiftDynamicPropertyInformation(QScriptEngine *engine)
    : ScWrappedClassNoInheritance<SDynamicPropertyInformation, ScShiftDynamicPropertyInformation>(engine, "Object")
  {
  addMemberProperty("parentType", parentType);
  addMemberProperty("version", version);
  addMemberProperty("type", type);

  _compute = SPropertyInstanceInformation::newDataKey();

  setConstructor<ScShiftDynamicPropertyInformation>(EXPOSED_NAME, construct);
  }

QScriptValue ScShiftDynamicPropertyInformation::construct(QScriptContext *ctx, QScriptEngine *eng)
  {
  QScriptValue arg = ctx->argument(0);
  if(arg.instanceOf(ctx->callee()))
    {
    // copy
    return packDefaultValue(eng, *unpackValue(arg));
    }
  else
    {
    // default
    QScriptValue arg1 = ctx->argument(1);

    SDynamicPropertyInformation info(arg.toString(), arg1.toUInt32());

    return packDefaultValue(eng, info);
    }
  return QScriptValue();
  }

QScriptValue ScShiftDynamicPropertyInformation::parentType(QScriptContext *ctx, QScriptEngine *eng)
  {
  SDynamicPropertyInformation *info = getThis(ctx);

  if (ctx->argumentCount() == 1)
    {
    // set property
    QScriptValue arg = ctx->argument(0);
    if(arg.isString())
      {
      info->setParentTypeName(arg.toString());
      }
    else
      {
      const SDynamicPropertyInformation *cls = unpackValue(arg);
      if (cls && cls->isValid())
        {
        info->setParentTypeName(cls->typeName());
        }
      }
    }

  return info->parentTypeName();
  }

QScriptValue ScShiftDynamicPropertyInformation::version(QScriptContext *ctx, QScriptEngine *eng)
  {
  SDynamicPropertyInformation *info = getThis(ctx);

  if (ctx->argumentCount() == 1)
    {
    // set property
    QScriptValue arg = ctx->argument(0);
    if(arg.isNumber())
      {
      info->setVersion(arg.toNumber());
      }
    }

  return info->version();
  }


QScriptValue ScShiftDynamicPropertyInformation::type(QScriptContext *ctx, QScriptEngine *eng)
  {
  SDynamicPropertyInformation *info = getThis(ctx);

  if (ctx->argumentCount() == 1)
    {
    // set property
    QScriptValue arg = ctx->argument(0);
    if(arg.isString())
      {
      info->setTypeName(arg.toString());
      }
    }

  return info->typeName();
  }

void ScShiftDynamicPropertyInformation::computeFn(SPropertyContainer *, SProperty *)
  {
  }
