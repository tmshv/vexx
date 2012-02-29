#include "XInterface.h"

XInterfaceBase::XInterfaceBase(xsize typeId,
                               const char *typeName,
                               v8::Handle<v8::Value> ctor(v8::Arguments const &argv),
                               xsize typeIdField,
                               xsize nativeField,
                               xsize internalFieldCount)
  : _constructor(v8::Persistent<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(ctor))),
    _prototype(v8::Persistent<v8::ObjectTemplate>::New(_constructor->PrototypeTemplate())),
    _typeName(typeName),
    _typeId(typeId),
    _typeIdField(typeIdField),
    _nativeField(nativeField),
    _isSealed(false)
  {
  _constructor->InstanceTemplate()->SetInternalFieldCount(internalFieldCount);
  }

void XInterfaceBase::seal()
  {
  _isSealed = true;
  }

v8::Handle<v8::Function> XInterfaceBase::constructorFunction() const
  {
  // In my experience, if GetFunction() is called BEFORE setting up
  // the Prototype object, v8 gets very unhappy (class member lookups don't work?).
  _isSealed = true;
  return _constructor->GetFunction();
  }

void XInterfaceBase::wrapInstance(v8::Handle<v8::Object> obj, void *object) const
  {
  if( 0 <= _typeIdField )
    {
    xAssert(_typeIdField < (xsize)obj->InternalFieldCount());
    obj->SetPointerInInternalField(_typeIdField, (void*)_typeId);
    }
  xAssert(_nativeField < (xsize)obj->InternalFieldCount());
  obj->SetPointerInInternalField(_nativeField, object);
}

void XInterfaceBase::unwrapInstance(v8::Handle<v8::Object> object) const
  {
  xAssert(_nativeField < (xsize)object->InternalFieldCount());
  object->SetInternalField(_nativeField, v8::Null());
  if(0 <= _typeIdField)
    {
    xAssert(_typeIdField < (xsize)object->InternalFieldCount());
    object->SetInternalField(_typeIdField, v8::Null());
    }
  }

v8::Handle<v8::Object> XInterfaceBase::newInstance(int argc, v8::Handle<v8::Value> argv[]) const
  {
  return constructorFunction()->NewInstance(argc, argv);
  }

v8::Handle<v8::Object> XInterfaceBase::newInstanceBase() const
  {
  return _constructor->InstanceTemplate()->NewInstance();
  }

void XInterfaceBase::set(const char *name, v8::Handle<v8::Value> val)
  {
  _prototype->Set(v8::String::NewSymbol(name), val);
  }

void XInterfaceBase::addProperty(const char *name, v8::AccessorGetter getter, v8::AccessorSetter setter)
  {
  _prototype->SetAccessor(v8::String::New(name), getter, setter);
  }

void XInterfaceBase::addClassTo(const char *thisClassName, const v8::Handle<v8::Object> &dest) const
  {
  dest->Set(v8::String::NewSymbol(thisClassName), constructorFunction());
  }

void XInterfaceBase::inherit(XInterfaceBase *parentType)
  {
  if(!parentType->isSealed())
  {
      std::ostringstream os;
      os << "XInterfaceBase<"
         << parentType->typeName()
         << "> has not been sealed yet!";
      throw std::runtime_error(os.str());
  }
  constructor()->Inherit( parentType->constructor() );
  }
