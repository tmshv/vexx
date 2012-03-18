#include "XInterface.h"
#include "XScriptValueV8Internals.h"
#include "XUnorderedMap"

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
                               const QString &typeName,
                               XScriptValue ctor(XScriptArguments const &argv),
                               xsize typeIdField,
                               xsize nativeField,
                               xsize internalFieldCount,
                               ToScriptFn convert)
  : _typeName(typeName),
    _typeId(typeId),
    _typeIdField(typeIdField),
    _nativeField(nativeField),
    _isSealed(false),
    _toScript(convert)
  {
  xAssert(_typeName.length());
  new(constructor(_constructor)) FnTempl(FnTempl::New(v8::FunctionTemplate::New((v8::InvocationCallback)ctor)));
  new(::prototype(_prototype)) ObjTempl(ObjTempl::New((*constructor(_constructor))->PrototypeTemplate()));

  (*constructor(_constructor))->InstanceTemplate()->SetInternalFieldCount(internalFieldCount);
  }

XInterfaceBase::~XInterfaceBase()
  {
  constructor(_constructor)->~FnTempl();
  ::prototype(_prototype)->~ObjTempl();
  }

XScriptValue XInterfaceBase::copyValue(const void *val) const
  {
  xAssert(val);

  if(_toScript)
    {
    return _toScript(val);
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

XScriptObject XInterfaceBase::newInstanceBase() const
  {
  return fromObjectHandle((*constructor(_constructor))->InstanceTemplate()->NewInstance());
  }

void XInterfaceBase::set(const char *name, XScriptValue val)
  {
  (*::prototype(_prototype))->Set(v8::String::NewSymbol(name), getV8Internal(val));
  }

void XInterfaceBase::addProperty(const char *name, Getter getter, Setter setter)
  {
  (*::prototype(_prototype))->SetAccessor(v8::String::New(name), (v8::AccessorGetter)getter, (v8::AccessorSetter)setter);
  }

void XInterfaceBase::addClassTo(const QString &thisClassName, const XScriptObject &dest) const
  {
  getV8Internal(dest)->Set(v8::String::NewSymbol(thisClassName.toAscii().constData()), getV8Internal(constructorFunction()));
  }

void XInterfaceBase::inherit(XInterfaceBase *parentType)
  {
  if(!parentType->isSealed())
    {
    throw std::runtime_error("XInterfaceBase<T> has not been sealed yet!");
    }
  FnTempl* templ = constructor(_constructor);
  FnTempl* pTempl = constructor(parentType->_constructor);
  (*templ)->Inherit( (*pTempl) );
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
void registerInterface(int id, XInterfaceBase *interface)
  {
  _interfaces.insert(id, interface);
  }

XInterfaceBase *findInterface(int id)
  {
  XInterfaceBase *base = _interfaces[id];
  xAssert(base->isSealed());
  return base;
  }
