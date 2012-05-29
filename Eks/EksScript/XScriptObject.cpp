#include "XScriptObject.h"
#include "XScriptValueV8Internals.h"
#include "XScriptValueDartInternals.h"

struct XScriptObjectInternal
  {
  static XScriptObjectInternal *init(XScriptObject *o, const XScriptObject *other=0)
    {
    XScriptObjectInternal *internal = (XScriptObjectInternal*)o;
    new(internal) XScriptObjectInternal;

    if(other)
      {
      const XScriptObjectInternal *otherInternal = (const XScriptObjectInternal*)other;
      internal->_object = otherInternal->_object;
      }

    return internal;
    }

  static void term(XScriptObject *o)
    {
    XScriptObjectInternal *internal = (XScriptObjectInternal*)o;
    (void)internal;
    internal->~XScriptObjectInternal();
    }

  static const XScriptObjectInternal *val(const XScriptObject *o)
    {
    XScriptObjectInternal *internal = (XScriptObjectInternal*)o;
    return internal;
    }

  static XScriptObjectInternal *val(XScriptObject *o)
    {
    XScriptObjectInternal *internal = (XScriptObjectInternal*)o;
    return internal;
    }

  mutable v8::Handle<v8::Object> _object;
  };
xCompileTimeAssert(sizeof(XScriptObject) == sizeof(XScriptObjectInternal));

Dart_Handle& getDartHandle(const XScriptObject &obj)
  {
  struct Internal
    {
    Dart_Handle ptr;
    };

  return ((Internal*)(&obj))->ptr;
  }

XScriptObject::XScriptObject()
  {
  XScriptObjectInternal::init(this);
  }

XScriptObject::~XScriptObject()
  {
  XScriptObjectInternal::term(this);
  }

XScriptObject::XScriptObject(const XScriptObject &other)
  {
  XScriptObjectInternal::init(this, &other);
  }

XScriptObject::XScriptObject(const XScriptValue &other)
  {
  const v8::Handle<v8::Value> otherInternal = getV8Internal(other);
  XScriptObjectInternal *internal = XScriptObjectInternal::init(this);
  if(getV8Internal(other)->IsObject())
    {
    internal->_object = v8::Handle<v8::Object>(v8::Object::Cast(*otherInternal));
    }
  }

XScriptObject::XScriptObject(const XScriptFunction &other)
  {
    v8::Handle<v8::Function> otherInternal = getV8Internal(other);
  XScriptObjectInternal *internal = XScriptObjectInternal::init(this);
  internal->_object = otherInternal;
  }

XScriptObject& XScriptObject::operator=(const XScriptObject &other)
  {
  XScriptObjectInternal::term(this);
  XScriptObjectInternal::init(this, &other);
  return *this;
  }

xsize XScriptObject::internalFieldCount() const
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  return internal->_object->InternalFieldCount();
  }

void *XScriptObject::internalField(xsize idx) const
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  return internal->_object->GetPointerFromInternalField(idx);
  }

XInterfaceBase *XScriptObject::getInterface() const
  {
  typedef XScript::ClassCreator_InternalFields_Base<void> IF;
  xsize tid = 0;
  XScriptValue proto(*this);
  while(!tid && proto.isObject())
    {
    XScriptObject const &obj(proto);
    tid = (obj.internalFieldCount() != IF::Count)
      ? 0
      : (xsize)obj.internalField(IF::TypeIDIndex);

    proto = obj.getPrototype();
    }

  return tid != 0 ? findInterface(tid) : 0;
  }

XScriptValue XScriptObject::getPrototype() const
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  return fromHandle(internal->_object->GetPrototype());
  }

void XScriptObject::setPrototype(XScriptValue proto)
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  internal->_object->SetPrototype(getV8Internal(proto));
  }

XScriptValue XScriptObject::get(const QString &n) const
  {
  XScriptValue key = XScriptConvert::to(n);
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  return fromHandle(internal->_object->Get(getV8Internal(key)));
  }

void XScriptObject::set(const QString &n, const XScriptValue &v)
  {
  XScriptValue key = XScriptConvert::to(n);
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  internal->_object->Set(getV8Internal(key), getV8Internal(v));
  }

bool XScriptObject::isValid() const
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  return !internal->_object.IsEmpty() && internal->_object->IsObject();
  }

XScriptObject XScriptObject::newObject()
  {
  XScriptObject obj;
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(&obj);
  internal->_object = v8::Object::New();
  return obj;
  }

v8::Handle<v8::Object> getV8Internal(const XScriptObject &o)
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(&o);
  return internal->_object;
  }

XScriptObject fromObjectHandle(v8::Handle<v8::Object> v)
  {
  XScriptObject o;
  XScriptObjectInternal *internal = XScriptObjectInternal::val(&o);
  internal->_object = v;
  return o;
  }

XScriptObject fromObjectHandle(Dart_Handle v)
  {
  XScriptObject o;
  getDartHandle(o) = v;
  return o;
  }

Dart_Handle getDartInternal(const XScriptObject &o)
  {
  return getDartHandle(o);
  }

