#ifndef SCSHIFTFLOATARRAYPROPERTY_H
#define SCSHIFTFLOATARRAYPROPERTY_H

#include "scshiftproperty.h"

class ScShiftFloatArrayProperty : public ScShiftProperty
  {
public:
  ScShiftFloatArrayProperty(QScriptEngine *engine, const QString &parentType="SProperty");
  ~ScShiftFloatArrayProperty();

  void initiate();

  static QScriptValue add(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue at(QScriptContext *ctx, QScriptEngine *);
  };

Q_DECLARE_METATYPE(ScShiftFloatArrayProperty*)

#endif // SCSHIFTENTITY_H
