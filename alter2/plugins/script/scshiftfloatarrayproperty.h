#ifndef SCSHIFTFLOATARRAYPROPERTY_H
#define SCSHIFTFLOATARRAYPROPERTY_H

#include "scshiftproperty.h"

class ScShiftFloatArrayProperty : public ScWrappedClass<SProperty *>
  {
public:
  ScShiftFloatArrayProperty(QScriptEngine *engine);
  ~ScShiftFloatArrayProperty();

  static QScriptValue add(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue at(QScriptContext *ctx, QScriptEngine *);
  };

Q_DECLARE_METATYPE(ScShiftFloatArrayProperty*)

#endif // SCSHIFTENTITY_H
