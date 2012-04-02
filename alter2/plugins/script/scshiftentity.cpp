#if 0
#include "scshiftentity.h"
#include "sentity.h"
#include "sdatabase.h"
#include "scembeddedtypes.h"
#include "styperegistry.h"

ScShiftEntity::ScShiftEntity(QScriptEngine *eng) : ScShiftPropertyContainerBase(eng)
  {
  addMemberFunction("addChild", addChild);
  initiateGlobalValue<ScShiftEntity>("SEntity", "SPropertyContainer");
  }

ScShiftEntity::~ScShiftEntity()
  {
  }

QScriptValue ScShiftEntity::addChild(QScriptContext *ctx, QScriptEngine *)
  {
  ScProfileFunction
  SProperty **propPtr = getThis(ctx);
  if(propPtr && ctx->argumentCount() >= 1)
    {
    QScriptValue typeValue = ctx->argument(0);
    QString type;
    if(typeValue.isObject())
      {
      type = typeValue.property("typeName").toString();
      }
    else
      {
      type = typeValue.toString();
      }

    QString name;
    if(ctx->argumentCount() >= 2)
      {
      QScriptValue arg2 = ctx->argument(1);
      if(arg2.isString())
        {
        name = ctx->argument(1).toString();
        }
      }
    SEntity *ent = (*propPtr)->uncheckedCastTo<SEntity>();
    if(ent)
      {
      const SPropertyInformation *info = STypeRegistry::findType(type);
      if(info)
        {
        return ScEmbeddedTypes::packValue(ent->addChild(info, name));
        }
      else
        {
        ctx->throwError(QScriptContext::SyntaxError, "Invalid type name " + type + "passed to SEntity.addChild(...);");
        return QScriptValue();
        }
      }
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect arguments to SEntity.addChild(...);");
  return QScriptValue();
  }
#endif
