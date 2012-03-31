#include "XInterface.h"
#include "XScriptValueV8Internals.h"
#include "XUnorderedMap"
#include "QUrl"

typedef v8::Persistent<v8::FunctionTemplate> FnTempl;
typedef v8::Persistent<v8::ObjectTemplate> ObjTempl;

FnTempl *constructor(void*& b)
  {
  return (FnTempl*)&b;
  }

ObjTempl *prototype(void*& b)
  {
  return (ObjTempl*)&b;
  }

const FnTempl *constructor(const void *const& b)
  {
  return (const FnTempl*)&b;
  }

const ObjTempl *prototype(const void *& b)
  {
  return (const ObjTempl*)&b;
  }

XInterfaceBase::XInterfaceBase(xsize typeId,
                               xsize nonPointerTypeId,
                               const QString &typeName,
                               XScriptValue ctor(XScriptArguments const &argv),
                               xsize typeIdField,
                               xsize nativeField,
                               xsize internalFieldCount,
                               const XInterfaceBase *parent,
                               ToScriptFn tScr,
                               FromScriptFn fScr)
  : _typeName(typeName),
    _typeId(typeId),
    _nonPointerTypeId(nonPointerTypeId),
    _typeIdField(typeIdField),
    _nativeField(nativeField),
    _isSealed(false),
    _parent(parent),
    _toScript(tScr),
    _fromScript(fScr)
  {
  xAssert(_typeName.length());
  new(constructor(_constructor)) FnTempl(FnTempl::New(v8::FunctionTemplate::New((v8::InvocationCallback)ctor)));
  new(::prototype(_prototype)) ObjTempl(ObjTempl::New((*constructor(_constructor))->PrototypeTemplate()));

  v8::Handle<v8::String> typeNameV8 = v8::String::New("typeName");
  v8::Handle<v8::String> typeNameStrV8 = v8::String::New((uint16_t*)typeName.constData(), typeName.length());
  (*constructor(_constructor))->Set(typeNameV8, typeNameStrV8);

  (*constructor(_constructor))->InstanceTemplate()->SetInternalFieldCount(internalFieldCount);

  if(parent)
    {
    inherit(parent);
    }
  }

XInterfaceBase::~XInterfaceBase()
  {
  constructor(_constructor)->~FnTempl();
  ::prototype(_prototype)->~ObjTempl();
  }

QVariant XInterfaceBase::toVariant(const XScriptValue &inp, int typeHint)
  {
  if(_fromScript)
    {
    const void *val = _fromScript(inp);

    if(typeHint == _typeId)
      {
      return QVariant(_typeId, &val);
      }
    else if(typeHint == _nonPointerTypeId)
      {
      return QVariant(_nonPointerTypeId, val);
      }

    UpCastFn fn = _upcasts.value(typeHint, 0);
    if(fn)
      {
      void *typedPtr = fn((void*)val);
      return QVariant(typeHint, &typedPtr);
      }
    }

  return QVariant();
  }

XScriptValue XInterfaceBase::copyValue(const QVariant &v) const
  {
  int type = v.userType();
  // T Ptr (to either a pointer or the actual type)
  const void **val = (const void**)v.constData();
  xAssert(val);

  if(_toScript)
    {
    if(type == _typeId)
      {
      // resolve to T Ptr
      return _toScript(*val);
      }
    else if(type == _nonPointerTypeId)
      {
      // already a pointer to T
      return _toScript(val);
      }
    }

  return XScriptValue();
  }

void XInterfaceBase::seal()
  {
  _isSealed = true;
  }

XScriptFunction XInterfaceBase::constructorFunction() const
  {
  // In my experience, if GetFunction() is called BEFORE setting up
  // the Prototype object, v8 gets very unhappy (class member lookups don't work?).
  _isSealed = true;
  return fromFunction((*constructor(_constructor))->GetFunction());
  }

void XInterfaceBase::wrapInstance(XScriptObject scObj, void *object) const
  {
  xAssert(findInterface(_typeId));
  v8::Handle<v8::Object> obj = getV8Internal(scObj);
  if( 0 <= _typeIdField )
    {
    xAssert(_typeIdField < (xsize)obj->InternalFieldCount());
    obj->SetPointerInInternalField(_typeIdField, (void*)_typeId);
    }
  xAssert(_nativeField < (xsize)obj->InternalFieldCount());
  obj->SetPointerInInternalField(_nativeField, object);
  }

void XInterfaceBase::unwrapInstance(XScriptObject scObj) const
  {
  v8::Handle<v8::Object> object = getV8Internal(scObj);
  xAssert(_nativeField < (xsize)object->InternalFieldCount());
  object->SetInternalField(_nativeField, v8::Null());
  if(0 <= _typeIdField)
    {
    xAssert(_typeIdField < (xsize)object->InternalFieldCount());
    object->SetInternalField(_typeIdField, v8::Null());
    }
  }

XScriptObject XInterfaceBase::newInstance(int argc, XScriptValue argv[]) const
  {
  return fromObjectHandle(getV8Internal(constructorFunction())->NewInstance(argc, getV8Internal(argv)));
  }

void XInterfaceBase::set(const char *name, XScriptValue val)
  {
  (*::prototype(_prototype))->Set(v8::String::NewSymbol(name), getV8Internal(val));
  }

void XInterfaceBase::addProperty(const char *name, Getter getter, Setter setter)
  {
  (*::prototype(_prototype))->SetAccessor(v8::String::New(name), (v8::AccessorGetter)getter, (v8::AccessorSetter)setter);
  }

void XInterfaceBase::addFunction(const char *name, Function fn)
  {
  v8::Handle<v8::FunctionTemplate> fnTmpl = ::v8::FunctionTemplate::New((v8::InvocationCallback)fn);
  (*::prototype(_prototype))->Set(v8::String::New(name), fnTmpl->GetFunction());
  }

void XInterfaceBase::setCallableAsFunction(Function fn)
  {
  xAssertFail();
  (*::prototype(_prototype))->SetCallAsFunctionHandler((v8::InvocationCallback)fn);
  }

void XInterfaceBase::addClassTo(const QString &thisClassName, const XScriptObject &dest) const
  {
  getV8Internal(dest)->Set(v8::String::NewSymbol(thisClassName.toAscii().constData()), getV8Internal(constructorFunction()));
  }

void XInterfaceBase::inherit(const XInterfaceBase *parentType)
  {
  if(!parentType->isSealed())
    {
    throw std::runtime_error("XInterfaceBase<T> has not been sealed yet!");
    }
  FnTempl* templ = constructor(_constructor);
  const FnTempl* pTempl = constructor(parentType->_constructor);
  (*templ)->Inherit( (*pTempl) );
  }

void XInterfaceBase::addChildInterface(int typeId, UpCastFn fn)
  {
  xAssert(!_upcasts.contains(typeId));
  _upcasts.insert(typeId, fn);
  }

void *XInterfaceBase::prototype()
  {
  return _prototype;
  }

v8::Handle<v8::ObjectTemplate> getV8Internal(XInterfaceBase *o)
  {
  void *proto = o->prototype();
  return *::prototype(proto);
  }

XUnorderedMap<int, XInterfaceBase*> _interfaces;
void registerInterface(XInterfaceBase *interface)
  {
  xAssert(!_interfaces.contains(interface->typeId()));
  _interfaces.insert(interface->typeId(), interface);
  if(interface->nonPointerTypeId() != QVariant::Invalid)
    {
    _interfaces.insert(interface->nonPointerTypeId(), interface);
    }
  }

XInterfaceBase *findInterface(int id)
  {
#ifdef X_DEBUG
  QVariant::Type type = (QVariant::Type)id;
  (void)type;
  const char *typeName = QMetaType::typeName(type);
  (void)typeName;
#endif

  XInterfaceBase *base = _interfaces[id];
  xAssert(!base || base->isSealed());
  return base;
  }
