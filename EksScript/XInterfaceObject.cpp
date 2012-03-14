#include "XInterfaceObject.h"
#include "XScriptObjectV8Internals.h"

struct XInterfaceObjectInternal
  {
  static XInterfaceObjectInternal *init(XInterfaceObject *o, const XInterfaceObject *other=0)
    {
    XInterfaceObjectInternal *internal = (XInterfaceObjectInternal*)o;
    new(internal) XInterfaceObjectInternal;

    if(other)
      {
      const XInterfaceObjectInternal *otherInternal = (const XInterfaceObjectInternal*)other;
      internal->_object = otherInternal->_object;
      }

    return internal;
    }

  static void term(XInterfaceObject *o)
    {
    XInterfaceObjectInternal *internal = (XInterfaceObjectInternal*)o;
    (void)internal;
    internal->~XInterfaceObjectInternal();
    }

  static const XInterfaceObjectInternal *val(const XInterfaceObject *o)
    {
    XInterfaceObjectInternal *internal = (XInterfaceObjectInternal*)o;
    return internal;
    }

  static XInterfaceObjectInternal *val(XInterfaceObject *o)
    {
    XInterfaceObjectInternal *internal = (XInterfaceObjectInternal*)o;
    return internal;
    }

  mutable v8::Handle<v8::Object> _object;
  };
xCompileTimeAssert(sizeof(XInterfaceObject) == sizeof(XInterfaceObjectInternal));

XInterfaceObject::XInterfaceObject()
  {
  XInterfaceObjectInternal::init(this);
  }

XInterfaceObject::~XInterfaceObject()
  {
  XInterfaceObjectInternal::term(this);
  }

XInterfaceObject::XInterfaceObject(const XInterfaceObject &other)
  {
  XInterfaceObjectInternal::init(this, &other);
  }

XInterfaceObject::XInterfaceObject(const XScriptObject &other)
  {
  const v8::Handle<v8::Value> otherInternal = getV8Internal(other);
  XInterfaceObjectInternal *internal = XInterfaceObjectInternal::init(this);
  internal->_object = v8::Handle<v8::Object>(v8::Object::Cast(*otherInternal));
  }

XInterfaceObject& XInterfaceObject::operator=(const XInterfaceObject &other)
  {
  XInterfaceObjectInternal::term(this);
  XInterfaceObjectInternal::init(this, &other);
  return *this;
  }

xsize XInterfaceObject::internalFieldCount() const
  {
  const XInterfaceObjectInternal *internal = XInterfaceObjectInternal::val(this);
  return internal->_object->InternalFieldCount();
  }

void *XInterfaceObject::internalField(xsize idx) const
  {
  const XInterfaceObjectInternal *internal = XInterfaceObjectInternal::val(this);
  return internal->_object->GetPointerFromInternalField(idx);
  }

XScriptObject XInterfaceObject::getPrototype() const
  {
  const XInterfaceObjectInternal *internal = XInterfaceObjectInternal::val(this);
  return fromHandle(internal->_object->GetPrototype());
  }

bool XInterfaceObject::isValid() const
  {
  const XInterfaceObjectInternal *internal = XInterfaceObjectInternal::val(this);
  return !internal->_object.IsEmpty();
  }

void XInterfaceObject::makeWeak(void *data, WeakDtor cb)
  {
  const XInterfaceObjectInternal *internal = XInterfaceObjectInternal::val(this);
  v8::Persistent<v8::Object> self( v8::Persistent<v8::Object>::New(internal->_object) );
  self.MakeWeak(data, (v8::WeakReferenceCallback)cb);
  }

v8::Handle<v8::Object> getV8Internal(const XInterfaceObject &o)
  {
  const XInterfaceObjectInternal *internal = XInterfaceObjectInternal::val(&o);
  return internal->_object;
  }

v8::Handle<v8::Value> *getV8Internal(const XScriptObject *o)
  {
  return (v8::Handle<v8::Value> *)o;
  }

XInterfaceObject fromObjectHandle(v8::Handle<v8::Object> v)
  {
  XInterfaceObject o;
  XInterfaceObjectInternal *internal = XInterfaceObjectInternal::val(&o);
  internal->_object = v;
  return o;
  }

