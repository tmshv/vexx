#include "XInterface.h"
#include "XScriptValueDartInternals.h"
#include "XScriptValueV8Internals.h"
#include "XUnorderedMap"
#include "QUrl"

#ifdef X_DART
# if 0
void dtor(Dart_Handle, void *peer)
{
  Nummer *num = (Nummer*)peer;
  delete num;
}

void ctor(Dart_NativeArguments arguments)
{
  int argsC = Dart_GetNativeArgumentCount(arguments);
  Dart_Handle ths = Dart_GetNativeArgument(arguments, 0);
  Dart_Handle arg = Dart_GetNativeArgument(arguments, 1);

  Nummer* num = new Nummer;
  Dart_NewWeakPersistentHandle(ths, num, dtor);

  Dart_DoubleValue(arg, &num->num);

  Dart_SetNativeInstanceField(ths, 0, (intptr_t)num);
}

const char* lib =
  "#library(\"NativeTest\");\n"
  "class NativeTest extends NativeWrapper {\n"
  "   NativeTest(Dynamic a) { _ctor(a); }"
  "   void _ctor(Dynamic a) native 'Fields_ctor';"
  "   Dynamic crapSalad(Dynamic) native 'Fields_crapSalad';"
  "}\n";

_lookup[std::pair<std::string, uint8_t>("Fields_ctor", 2)] = ctor;
_lookup[std::pair<std::string, uint8_t>("Fields_crapSalad", 2)] = crapSalad;

_libs["NativeTest"] = Dart_NewString(lib);
# endif
#else
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
#endif


XInterfaceBase::XInterfaceBase(xsize typeID,
               xsize nonPointerTypeID,
               const QString &typeName,
               const XInterfaceBase *parent)
  : _typeName(typeName),
    _typeId(typeID),
    _nonPointerTypeId(nonPointerTypeID),
    _internalFieldCount(parent->_internalFieldCount),
    _baseTypeId(parent->_baseTypeId),
    _baseNonPointerTypeId(parent->_baseNonPointerTypeId),
    _typeIdField(parent->_typeIdField),
    _nativeField(parent->_nativeField),
    _isSealed(true),
    _parent(parent),
    _toScript(parent->_toScript),
    _fromScript(parent->_fromScript)
  {
  xAssert(_typeName.length());

#ifndef X_DART
  new(constructor(_constructor)) FnTempl(FnTempl::New(v8::FunctionTemplate::New((v8::InvocationCallback)parent->_nativeCtor)));
  new(::prototype(_prototype)) ObjTempl(ObjTempl::New((*constructor(_constructor))->PrototypeTemplate()));

  v8::Handle<v8::String> typeNameV8 = v8::String::New("typeName");
  v8::Handle<v8::String> typeNameStrV8 = v8::String::New((uint16_t*)typeName.constData(), typeName.length());
  (*constructor(_constructor))->Set(typeNameV8, typeNameStrV8);
  (*::prototype(_prototype))->Set(typeNameV8, typeNameStrV8);

  (*constructor(_constructor))->InstanceTemplate()->SetInternalFieldCount(_internalFieldCount);
#endif

  if(parent)
    {
    inherit(parent);
    }
  }

XInterfaceBase::XInterfaceBase(xsize typeId,
                               xsize nonPointerTypeId,
                               xsize baseTypeId,
                               xsize baseNonPointerTypeId,
                               const QString &typeName,
                               NativeCtor ctor,
                               xsize typeIdField,
                               xsize nativeField,
                               xsize internalFieldCount,
                               const XInterfaceBase *parent,
                               ToScriptFn tScr,
                               FromScriptFn fScr)
  : _typeName(typeName),
    _typeId(typeId),
    _nonPointerTypeId(nonPointerTypeId),
    _internalFieldCount(internalFieldCount),
    _baseTypeId(baseTypeId),
    _baseNonPointerTypeId(baseNonPointerTypeId),
    _typeIdField(typeIdField),
    _nativeField(nativeField),
    _isSealed(false),
    _parent(parent),
    _toScript(tScr),
    _fromScript(fScr),
    _nativeCtor(ctor)
  {
  xAssert(_typeName.length());

#ifndef X_DART
  new(constructor(_constructor)) FnTempl(FnTempl::New(v8::FunctionTemplate::New((v8::InvocationCallback)ctor)));
  new(::prototype(_prototype)) ObjTempl(ObjTempl::New((*constructor(_constructor))->PrototypeTemplate()));

  v8::Handle<v8::String> typeNameV8 = v8::String::New("typeName");
  v8::Handle<v8::String> typeNameStrV8 = v8::String::New((uint16_t*)typeName.constData(), typeName.length());
  (*constructor(_constructor))->Set(typeNameV8, typeNameStrV8);
  (*::prototype(_prototype))->Set(typeNameV8, typeNameStrV8);

  (*constructor(_constructor))->InstanceTemplate()->SetInternalFieldCount(_internalFieldCount);
#endif

  if(parent)
    {
    inherit(parent);
    }
  }

XInterfaceBase::~XInterfaceBase()
  {
#ifndef X_DART
  constructor(_constructor)->~FnTempl();
  ::prototype(_prototype)->~ObjTempl();
#endif
  }

QVariant XInterfaceBase::toVariant(const XScriptValue &inp, int typeHint)
  {
  if(_fromScript)
    {
    const void *val = _fromScript(inp);

    if(typeHint == _typeId || typeHint == 0)
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

#ifndef X_DART
  return fromFunction((*constructor(_constructor))->GetFunction());
#endif
  return fromFunction(Dart_Null());
  }

void XInterfaceBase::wrapInstance(XScriptObject scObj, void *object) const
  {
#ifdef X_DART
  Dart_Handle obj = getDartInternal(scObj);
  if( 0 <= _typeIdField )
    {
    Dart_SetNativeInstanceField(obj, _typeIdField, (intptr_t)_baseTypeId);
    }
  Dart_SetNativeInstanceField(obj, _nativeField, (intptr_t)object);

#else
  xAssert(findInterface(_baseTypeId));
  v8::Handle<v8::Object> obj = getV8Internal(scObj);
  if( 0 <= _typeIdField )
    {
    xAssert(_typeIdField < (xsize)obj->InternalFieldCount());
    obj->SetPointerInInternalField(_typeIdField, (void*)_baseTypeId);
    }
  xAssert(_nativeField < (xsize)obj->InternalFieldCount());
  obj->SetPointerInInternalField(_nativeField, object);
#endif
  }

void XInterfaceBase::unwrapInstance(XScriptObject scObj) const
  {
#ifdef X_DART
  Dart_Handle obj = getDartInternal(scObj);
  if( 0 <= _typeIdField )
    {
    Dart_SetNativeInstanceField(obj, _typeIdField, (intptr_t)0);
    }
  Dart_SetNativeInstanceField(obj, _nativeField, (intptr_t)0);

#else
  v8::Locker l;
  v8::Handle<v8::Object> object = getV8Internal(scObj);
  xAssert(_nativeField < (xsize)object->InternalFieldCount());
  object->SetInternalField(_nativeField, v8::Null());
  if(0 <= _typeIdField)
    {
    xAssert(_typeIdField < (xsize)object->InternalFieldCount());
    object->SetInternalField(_typeIdField, v8::Null());
    }
#endif
  }

XScriptObject XInterfaceBase::newInstance(int argc, XScriptValue argv[]) const
  {
#ifdef X_DART
  xAssertFail();
  return fromObjectHandle(Dart_Null());

#else
  v8::Locker l;
  v8::Handle<v8::Object> newObj = getV8Internal(constructorFunction())->NewInstance(argc, getV8Internal(argv));

  v8::Handle<v8::Value> proto = newObj->GetPrototype();
  xAssert(!proto.IsEmpty());
  xAssert(proto->IsObject());

  return fromObjectHandle(newObj);
#endif
  }

void XInterfaceBase::set(const char *name, XScriptValue val)
  {
#ifdef X_DART
  xAssertFail();
#else
  (*::prototype(_prototype))->Set(v8::String::NewSymbol(name), getV8Internal(val));
#endif
  }

void XInterfaceBase::addProperty(const char *name, Getter getter, Setter setter)
  {
#ifdef X_DART
  xAssertFail();
#else
  (*::prototype(_prototype))->SetAccessor(v8::String::New(name), (v8::AccessorGetter)getter, (v8::AccessorSetter)setter);
#endif
  }

void XInterfaceBase::addFunction(const char *name, xsize argCount, Function fn, FunctionDart fnDart)
  {
#ifdef X_DART
  xAssertFail();
#else
  v8::Handle<v8::FunctionTemplate> fnTmpl = ::v8::FunctionTemplate::New((v8::InvocationCallback)fn);
  (*::prototype(_prototype))->Set(v8::String::New(name), fnTmpl->GetFunction());
#endif
  }

void XInterfaceBase::setIndexAccessor(IndexedGetter g)
  {
#ifdef X_DART
  xAssertFail();
#else
  (*::prototype(_prototype))->SetIndexedPropertyHandler((v8::IndexedPropertyGetter)g);
#endif
  }

void XInterfaceBase::setNamedAccessor(NamedGetter g)
  {
#ifdef X_DART
  xAssertFail();
#else
  (*::prototype(_prototype))->SetFallbackPropertyHandler((v8::NamedPropertyGetter)g);
#endif
  }

void XInterfaceBase::addClassTo(const QString &thisClassName, const XScriptObject &dest) const
  {
#ifdef X_DART
  xAssertFail();
#else
  getV8Internal(dest)->Set(v8::String::NewSymbol(thisClassName.toAscii().constData()), getV8Internal(constructorFunction()));
#endif
  }

void XInterfaceBase::inherit(const XInterfaceBase *parentType)
  {
  if(!parentType->isSealed())
    {
    throw std::runtime_error("XInterfaceBase<T> has not been sealed yet!");
    }
#ifdef X_DART
  xAssertFail();
#else
  FnTempl* templ = constructor(_constructor);
  const FnTempl* pTempl = constructor(parentType->_constructor);
  (*templ)->Inherit( (*pTempl) );
#endif
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
XUnorderedMap<int, XInterfaceBase*> _interfaces;
void registerInterface(int id, int nonPtrId, XInterfaceBase *interface)
  {
  xAssert(!_interfaces.contains(id));
  xAssert(!_interfaces.contains(nonPtrId));
  xAssert(id != QVariant::Invalid);

  _interfaces.insert(id, interface);
  if(nonPtrId != QVariant::Invalid)
    {
    _interfaces.insert(nonPtrId, interface);
    }
  }

void registerInterface(XInterfaceBase *interface)
  {
  registerInterface(interface->typeId(), interface->nonPointerTypeId(), interface);
  }

XInterfaceBase *findInterface(int id)
  {
#ifdef X_DEBUG
  QVariant::Type type = (QVariant::Type)id;
  (void)type;
  const char *typeName = QMetaType::typeName(type);
  (void)typeName;
#endif

  xAssert(id != QVariant::Invalid);
  XInterfaceBase *base = _interfaces.value(id);
  xAssert(!base || base->isSealed());
  return base;
  }


#ifndef X_DART
v8::Handle<v8::ObjectTemplate> getV8Internal(XInterfaceBase *o)
  {
  void *proto = o->prototype();
  return *::prototype(proto);
  }
#endif
