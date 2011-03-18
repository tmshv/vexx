#ifndef SCSHIFTDATABASE_H
#define SCSHIFTDATABASE_H

#include "scshiftentity.h"

class ScShiftDatabase : public ScShiftEntity
  {
public:
  ScShiftDatabase(QScriptEngine *engine);
  ~ScShiftDatabase();

  void initiate();

private:
  static QScriptValue addType(QScriptContext *ctx, QScriptEngine *);
  };

Q_DECLARE_METATYPE(ScShiftDatabase*)

#endif // SCSHIFTDATABASE_H
