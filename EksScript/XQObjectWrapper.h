#ifndef XQOBJECTWRAPPER_H
#define XQOBJECTWRAPPER_H

#include "XUnorderedMap"
#include "XInterface.h"
#include "XScriptObject.h"
#include "XScriptEngine.h"

class XQObjectConnectionList;

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
  XUnorderedMap<QObject *, XQObjectConnectionList *> _connections;
  friend class XQObjectConnectionList;
  friend struct Utils;
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
template <> class ClassCreator_Factory<QObject> : public ClassCreatorConvertableFactory<QObject> {};
}

#endif // XQOBJECTWRAPPER_H
