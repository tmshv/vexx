#ifndef SCSHIFTDATABASE_H
#define SCSHIFTDATABASE_H

#include "scglobal.h"
#include "scshiftentity.h"

class SPropertyInstanceInformation;
class SPropertyContainer;

class ScShiftDatabase : public ScShiftEntity
  {
public:
  ScShiftDatabase(QScriptEngine *engine);
  ~ScShiftDatabase();

  void initiate();

private:
  static QScriptValue addType(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue save(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue load(QScriptContext *ctx, QScriptEngine *);
  static void computeNode(const SPropertyInstanceInformation *instanceInfo, SPropertyContainer *node);
  };

Q_DECLARE_METATYPE(ScShiftDatabase*)
Q_DECLARE_METATYPE(QScriptValue)

#endif // SCSHIFTDATABASE_H
