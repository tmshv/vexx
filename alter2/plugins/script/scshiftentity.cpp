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
  ScProfileFunction
  SProperty **propPtr = getThis(ctx);
  if(propPtr && ctx->argumentCount() >= 1)
    {
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
      const SPropertyInformation *info = ent->database()->findType(ctx->argument(0).toString());
      if(info)
        {
        return ScEmbeddedTypes::packValue(ent->addChild(info->typeId(), name));
        }
      else
        {
        ctx->throwError(QScriptContext::SyntaxError, "Invalid type name " + ctx->argument(0).toString() + "passed to SEntity.addChild(...);");
        return QScriptValue();
        }
      }
    }
  ctx->throwError(QScriptContext::SyntaxError, "Incorrect arguments to SEntity.addChild(...);");
  return QScriptValue();
  }
