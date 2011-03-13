#ifndef SCSHIFTPROPERTY_H
#define SCSHIFTPROPERTY_H

#include "scwrappedclass.h"
class SProperty;

class ScShiftProperty : public ScWrappedClass<SProperty *, ScShiftProperty>
  {
public:
  ScShiftProperty(QScriptEngine *engine);

  void initiate();

  static QScriptValue input(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue outputs(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue firstOutput(QScriptContext *ctx, QScriptEngine *);
  };

Q_DECLARE_METATYPE(SProperty*)
Q_DECLARE_METATYPE(SProperty**)
Q_DECLARE_METATYPE(ScShiftProperty*)

#endif // SCSHIFTPROPERTY_H
