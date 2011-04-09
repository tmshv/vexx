#include "scshiftfloatarrayproperty.h"
#include "sarrayproperty.h"
#include "sentity.h"
#include "sdatabase.h"
#include "scembeddedtypes.h"

ScShiftFloatArrayProperty::ScShiftFloatArrayProperty(QScriptEngine *eng, const QString &parent) : ScShiftProperty(eng, parent)
  {
  addMemberFunction("add", add);
  }

ScShiftFloatArrayProperty::~ScShiftFloatArrayProperty()
  {
  }

void ScShiftFloatArrayProperty::initiate()
  {
  setBlankConstructor<ScShiftFloatArrayProperty>("ScShiftFloatArrayProperty");
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

