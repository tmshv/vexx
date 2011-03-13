#ifndef SCWRAPPEDCLASS_H
#define SCWRAPPEDCLASS_H

#include "QScriptClass"
#include "QScriptEngine"
#include "XGlobal"
#include "XProperty"

class ScPlugin;
class QScriptContext;

template <typename T, typename U> class ScWrappedClass : public QScriptClass
  {
  XWOProperty(QScriptValue, prototype);
  XWOProperty(QScriptValue, constructor);
  XROProperty(QScriptEngine *, engine);

public:
  ScWrappedClass(QScriptEngine *eng, const QString &parentType)
      : QScriptClass(eng), _engine(eng)
    {
    _prototype = engine()->newObject();

    // init our prototypes parent class
    QScriptValue globalObject = engine()->globalObject();
    _prototype.setPrototype(globalObject.property(parentType).property("prototype"));
    }

  void setConstructor(const QString &name, QScriptEngine::FunctionSignature constuctor)
    {
    QScriptValue globalObject = engine()->globalObject();
    _constructor = engine()->newFunction(constuctor, _prototype);
    _constructor.setData(qScriptValueFromValue(engine(), static_cast<U*>(this)));

    // register the constructor
    globalObject.setProperty(name, constructor());
    }

  void setBlankConstructor(const QString &name)
    {
    QScriptValue globalObject = engine()->globalObject();
    _constructor = engine()->newFunction(blank, _prototype);
    _constructor.setData(qScriptValueFromValue(engine(), static_cast<U*>(this)));

    // register the constructor
    globalObject.setProperty(name, constructor());
    }

  void addMemberFunction(const QString &str, QScriptEngine::FunctionSignature sig)
    {
    QScriptValue fn = engine()->newFunction(sig);
    _prototype.setProperty(str, fn);
    }

  void addMemberProperty(const QString &str, QScriptEngine::FunctionSignature sig, QScriptValue::PropertyFlags flags=QScriptValue::PropertyGetter|QScriptValue::PropertySetter)
    {
    _prototype.setProperty(str, engine()->newFunction(sig), flags);
    }

  virtual QScriptValue prototype() const
    {
    return _prototype;
    }

  QScriptValue constructor() const
    {
    return _constructor;
    }

  static T *getThis(QScriptContext *ctx)
    {
    T *t = unpackValue(ctx->thisObject());
    xAssert(t);
    return t;
    }

  static U *getWrappedClass(QScriptEngine *eng, const QString &name)
    {
    QScriptValue ctor = eng->globalObject().property(name);
    return qscriptvalue_cast<U*>(ctor.data());
    }

  static QScriptValue packValue(QScriptEngine *eng, const QString &name, const T &pack)
    {
    U *cls = getWrappedClass(eng, name);
    if(cls)
      {
      return eng->newObject(cls, eng->newVariant(qVariantFromValue(pack)));
      }
    return QScriptValue();
    }

  static T *unpackValue(const QScriptValue &val)
    {
    return qscriptvalue_cast<T*>(val.data());
    }

private:
  static QScriptValue blank(QScriptContext *ctx, QScriptEngine *)
    {
    return QScriptValue();
    }
  };

#endif // SCWRAPPEDCLASS_H
