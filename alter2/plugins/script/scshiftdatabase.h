#ifndef SCSHIFTDATABASE_H
#define SCSHIFTDATABASE_H

#include "scglobal.h"
#include "scshiftpropertycontainer.h"

class SPropertyInstanceInformation;
class SPropertyContainer;

class ScShiftDatabase : public ScShiftPropertyContainerBase
  {
public:
  ScShiftDatabase(QScriptEngine *engine);
  ~ScShiftDatabase();

private:
  static QScriptValue addType(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue save(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue load(QScriptContext *ctx, QScriptEngine *);
  static void computeNode(const SPropertyInstanceInformation *instanceInfo, SPropertyContainer *node);

  static void parseChildProperties(QScriptContext *ctx, SPropertyInformation *newType, QScriptValue propertiesArray);
  };

Q_DECLARE_METATYPE(ScShiftDatabase*)
Q_DECLARE_METATYPE(QScriptValue)

#endif // SCSHIFTDATABASE_H
