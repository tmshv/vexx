#ifndef SCWRAPPEDCLASS_H
#define SCWRAPPEDCLASS_H

#include "QScriptClass"
#include "QScriptEngine"
#include "XGlobal"
#include "XProperty"
#include "scglobal.h"
#include "sinterface.h"

class ScPlugin;
class QScriptContext;

class ScScriptInterface : public SStaticInterfaceBase
  {
  S_STATIC_INTERFACE_TYPE(ScScriptInterface, ScriptInterface)
public:
  ScScriptInterface(bool deleteOnNoReferences) : SStaticInterfaceBase(deleteOnNoReferences) { }
  virtual QScriptClass *scriptClass() = 0;
  };

template <typename T> class ScWrappedClass : public QScriptClass, public ScScriptInterface
  {
  XWOProperty(QScriptValue, prototype);
  XWOProperty(QScriptValue, constructor);
  XROProperty(QScriptEngine *, engine);

public:
  ScWrappedClass(QScriptEngine *eng, const QString &parentType)
      : QScriptClass(eng), ScScriptInterface(false), _engine(eng)
    {
    _prototype = engine()->newObject();
    }

//  template <typename U> void setConstructor(const QString &name, QScriptEngine::FunctionSignature constuctor, bool defaultValueForType=true)
//    {
//    QScriptValue globalObject = engine()->globalObject().property("db");
//    _constructor = engine()->newFunction(constuctor, _prototype);
//    _constructor.setData(qScriptValueFromValue(engine(), static_cast<U*>(this)));

//    // register the constructor
//    globalObject.setProperty(name, constructor());

//    if(defaultValueForType)
//      {
//      engine()->setDefaultPrototype(qMetaTypeId<T*>(), _prototype);
//      }
//    }

  template <typename U> void setDBValueConstructor(const QString &name)
    {
    QScriptValue globalObject = engine()->globalObject().property("db");
    _constructor = engine()->newFunction(blank, _prototype);
    _constructor.setData(qScriptValueFromValue(engine(), static_cast<U*>(this)));

    // register the constructor
    globalObject.setProperty(name, constructor());
    }

  void addMemberFunction(const QString &str, QScriptEngine::FunctionSignature sig)
    {
    _prototype.setProperty(str, engine()->newFunction(sig), QScriptValue::Undeletable|QScriptValue::SkipInEnumeration);
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
    return t;
    }

  static QScriptValue packValue(QScriptEngine *eng, QScriptClass *cls, const T &pack)
    {
    xAssert(cls);
    return eng->newObject(cls, eng->newVariant(qVariantFromValue(pack)));
    }

  static QScriptValue packDefaultValue(QScriptEngine *eng, const T &pack)
    {
    return eng->newVariant(qVariantFromValue(pack));
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
  virtual QScriptClass *scriptClass() { return this; }
  };

template <typename T, typename U> class ScWrappedClassNoInheritance : public ScWrappedClass<T>
  {
public:
  ScWrappedClassNoInheritance(QScriptEngine *eng, const QString &parentType)
      : ScWrappedClass<T>(eng, parentType)
    {
    }

  static U *getWrappedClass(QScriptEngine *eng, const QString &name)
    {
    QScriptValue ctor = eng->globalObject().property(name);
    return qscriptvalue_cast<U*>(ctor.data());
    }
  };

#endif // SCWRAPPEDCLASS_H
