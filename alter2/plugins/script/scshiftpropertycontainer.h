#ifndef SCSHIFTPROPERTYCONTAINER_H
#define SCSHIFTPROPERTYCONTAINER_H

#include "scshiftproperty.h"

class ScShiftPropertyContainerBase : public ScShiftPropertyBase
  {
public:
  ScShiftPropertyContainerBase(QScriptEngine *engine);
  ~ScShiftPropertyContainerBase();

  virtual QueryFlags queryProperty(const QScriptValue &object, const QScriptString &name, QueryFlags flags, uint *id);
  virtual QScriptValue property(const QScriptValue &object, const QScriptString &name, uint id);
  virtual void setProperty(QScriptValue &object, const QScriptString &name, uint id, const QScriptValue &value);
  virtual QScriptValue::PropertyFlags propertyFlags(const QScriptValue &object, const QScriptString &name, uint id);
  };

class ScShiftPropertyContainer : public ScShiftPropertyContainerBase
  {
public:
  ScShiftPropertyContainer(QScriptEngine *engine);
  ~ScShiftPropertyContainer();

  static QScriptValue size(QScriptContext *ctx, QScriptEngine *);
  };

Q_DECLARE_METATYPE(ScShiftPropertyContainer*)

#endif // SCSHIFTPROPERTYCONTAINER_H
