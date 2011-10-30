#include "scshiftfloatarrayproperty.h"
#include "sarrayproperty.h"
#include "sentity.h"
#include "sdatabase.h"
#include "scembeddedtypes.h"

ScShiftFloatArrayProperty::ScShiftFloatArrayProperty(QScriptEngine *eng) : ScWrappedClass<SProperty *>(eng)
  {
  addMemberFunction("add", add);
  addMemberFunction("at", at);
  initiateGlobalValue<ScShiftFloatArrayProperty>("SFloatArrayProperty", "SProperty");
  }

ScShiftFloatArrayProperty::~ScShiftFloatArrayProperty()
  {
  }

QScriptValue ScShiftFloatArrayProperty::add(QScriptContext *ctx, QScriptEngine *)
  {
  ScProfileFunction
  SProperty **thisProperty = getThis(ctx);
  SProperty **addAProperty = unpackValue(ctx->argument(0)); // returning script node as object

  SProperty **addBProperty = 0;
  if(ctx->argumentCount() > 1 )
    {
    addBProperty = unpackValue(ctx->argument(1)); // returning script node as object
    }

  if(addAProperty && thisProperty)
    {
    SFloatArrayProperty* addThis = (*thisProperty)->castTo<SFloatArrayProperty>();
    SFloatArrayProperty* addA = (*addAProperty)->castTo<SFloatArrayProperty>();
    SFloatArrayProperty* addB = (*addBProperty)->castTo<SFloatArrayProperty>();

    if(addThis && addA)
      {
      if(addB)
        {
        addThis->add(addA, addB);
        }
      else
        {
        addThis->add(addA);
        }
      }
    }
  return QScriptValue();
  }

QScriptValue ScShiftFloatArrayProperty::at(QScriptContext *ctx, QScriptEngine *)
  {
  ScProfileFunction
  SProperty **thisProperty = getThis(ctx);


  if(thisProperty && ctx->argumentCount() == 2)
    {
    SFloatArrayProperty* thisArray = (*thisProperty)->castTo<SFloatArrayProperty>();

    if(thisArray)
      {
      //quint32 x = ctx->argument(0).toUInt32();
      //quint32 y = ctx->argument(1).toUInt32();
      xAssertFail();
      return "";//thisArray->atIndex(x, y);
      }
    }
  return QScriptValue();
  }

