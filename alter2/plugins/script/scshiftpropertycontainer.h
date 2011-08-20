#ifndef SCSHIFTPROPERTYCONTAINER_H
#define SCSHIFTPROPERTYCONTAINER_H

#include "scshiftproperty.h"

class ScShiftPropertyContainer : public ScWrappedClass<SProperty *>
  {
public:
  ScShiftPropertyContainer(QScriptEngine *engine);
  ~ScShiftPropertyContainer();

  virtual QueryFlags queryProperty(const QScriptValue &object, const QScriptString &name, QueryFlags flags, uint *id);
  virtual QScriptValue property(const QScriptValue &object, const QScriptString &name, uint id);
  virtual void setProperty(QScriptValue &object, const QScriptString &name, uint id, const QScriptValue &value);
  virtual QScriptValue::PropertyFlags propertyFlags(const QScriptValue &object, const QScriptString &name, uint id);

  static QScriptValue size(QScriptContext *ctx, QScriptEngine *);

  void initiate();
  };

Q_DECLARE_METATYPE(ScShiftPropertyContainer*)

#endif // SCSHIFTPROPERTYCONTAINER_H
