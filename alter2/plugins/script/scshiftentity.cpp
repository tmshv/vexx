#include "scshiftentity.h"
#include "sentity.h"
#include "sdatabase.h"
#include "scembeddedtypes.h"

ScShiftEntity::ScShiftEntity(QScriptEngine *eng, const QString &parent) : ScShiftPropertyContainer(eng, parent)
  {
  addMemberFunction("addChild", addChild);
  }

ScShiftEntity::~ScShiftEntity()
  {
  }

void ScShiftEntity::initiate()
  {
  setBlankConstructor<ScShiftEntity>("SEntity");
  }

QScriptValue ScShiftEntity::addChild(QScriptContext *ctx, QScriptEngine *)
  {
  SProperty **propPtr = getThis(ctx);
  if(propPtr && ctx->argumentCount() >= 1)
    {
    SEntity *ent = (*propPtr)->uncheckedCastTo<SEntity>();
    if(ent)
      {
      const SPropertyInformation *info = ent->database()->findType(ctx->argument(0).toString());
      if(info)
        {
        return ScEmbeddedTypes::packValue(ent->addChild(info->typeId(), ctx->argument(1).toString()));
        }
      else
        {
        ctx->throwError(QScriptContext::TypeError, "Invalid type name passed to SEntity.addChild(...);");
        }
      }
    }
  ctx->throwError(QScriptContext::TypeError, "Incorrect arguments to SEntity.addChild(...);");
  return QScriptValue();
  }
