#include "scshiftdatabase.h"
#include "sdatabase.h"
#include "scembeddedtypes.h"

ScShiftDatabase::ScShiftDatabase(QScriptEngine *eng) : ScShiftEntity(eng, "SEntity")
  {
  addMemberFunction("addType", addType);
  }

ScShiftDatabase::~ScShiftDatabase()
  {
  }

void ScShiftDatabase::initiate()
  {
  setBlankConstructor<ScShiftDatabase>("SDatabase");
  }

QScriptValue ScShiftDatabase::addType(QScriptContext *ctx, QScriptEngine *engine)
  {
  // SPropertyInformation* info = new SPropertyInformation()
  return QScriptValue();
  }
