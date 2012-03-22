#ifndef XQOBJECTWRAPPER_H
#define XQOBJECTWRAPPER_H

#include "XUnorderedMap"
#include "XInterface.h"
#include "XScriptObject.h"
#include "XScriptEngine.h"

class EKSSCRIPT_EXPORT XQObjectWrapper
  {
public:
  static void initiate(XScriptEngine *);
  static XQObjectWrapper *instance();
  ~XQObjectWrapper();

  XScriptObject wrap(QObject *);
  XInterfaceBase *findInterface(const QMetaObject *object);

private:
  XQObjectWrapper();

  static void buildInterface(XInterfaceBase *interface, const QMetaObject *metaObject);

  XScriptEngine *_context;
  XUnorderedMap<const QMetaObject *, XInterfaceBase *> _objects;
  };

namespace XScriptConvert {
namespace internal {
template <> struct JSToNative<QObject> : XScriptConvert::internal::JSToNativeObject<QObject> {};

template <> struct NativeToJS<QObject>
  {
  XScriptValue operator()(QObject *n) const
    {
    return XQObjectWrapper::instance()->wrap(n);
    }
  XScriptValue operator()(QObject &n) const
    {
    xAssertFail();
    return this->operator()(&n);
    }
  };
}
}

namespace XScript
{
typedef XSignature< QObject (XScript::CtorForwarder<QObject *()> )> QObjectCtors;
template <> class ClassCreator_Factory<QObject> : public ClassCreatorCopyableFactory<QObject, QObjectCtors> {};
}

#endif // XQOBJECTWRAPPER_H
