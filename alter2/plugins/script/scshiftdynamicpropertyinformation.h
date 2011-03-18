#ifndef SCSHIFTDYNAMICPROPERTYINFORMATION_H
#define SCSHIFTDYNAMICPROPERTYINFORMATION_H

#include "sdynamicpropertyinformation.h"
#include "scwrappedclass.h"


class ScShiftDynamicPropertyInformation
    : public ScWrappedClassNoInheritance<SDynamicPropertyInformation, ScShiftDynamicPropertyInformation>
  {
public:
  ScShiftDynamicPropertyInformation(QScriptEngine *);

private:
  static QScriptValue construct(QScriptContext *ctx, QScriptEngine *);

  static QScriptValue parentType(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue version(QScriptContext *ctx, QScriptEngine *);
  static QScriptValue type(QScriptContext *ctx, QScriptEngine *);

  static void computeFn(SPropertyContainer *, SProperty *);

  SPropertyInstanceInformation::DataKey _compute;
  };

Q_DECLARE_METATYPE(SDynamicPropertyInformation)
Q_DECLARE_METATYPE(SDynamicPropertyInformation*)
Q_DECLARE_METATYPE(ScShiftDynamicPropertyInformation*)

#endif // SCSHIFTDYNAMICPROPERTYINFORMATION_H
