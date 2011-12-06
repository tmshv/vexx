#ifndef SCSHIFTPROPERTY_H
#define SCSHIFTPROPERTY_H

#include "scwrappedclass.h"
class SProperty;

class ScShiftProperty : public ScWrappedClass<SProperty *>
  {
public:
  ScShiftProperty(QScriptEngine *engine);
  ~ScShiftProperty();

  static QScriptValue typeInformation(QScriptContext *ctx, QScriptEngine *);

  static QScriptValue input(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue outputs(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue firstOutput(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue name(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue value(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue setValue(QScriptContext *ctx, QScriptEngine *);

  static QScriptValue registerExporter(QScriptContext *ctx, QScriptEngine *);
  };

Q_DECLARE_METATYPE(SProperty*)
Q_DECLARE_METATYPE(SProperty**)
Q_DECLARE_METATYPE(ScShiftProperty*)

#endif // SCSHIFTPROPERTY_H
