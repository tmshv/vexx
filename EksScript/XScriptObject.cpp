#include "XScriptObject.h"
#include "XScriptObjectV8Internals.h"
#include "XInterfaceObject.h"
#include "XAssert"

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
    const XScriptObjectInternal *internal = (XScriptObjectInternal*)o;
    return internal;
    }

  static XScriptObjectInternal *val(XScriptObject *o)
    {
    XScriptObjectInternal *internal = (XScriptObjectInternal*)o;
    return internal;
    }

  mutable v8::Handle<v8::Value> _object;
  };
xCompileTimeAssert(sizeof(XScriptObject) == sizeof(XScriptObjectInternal));

XScriptObject::XScriptObject()
  {
  XScriptObjectInternal *internal = XScriptObjectInternal::init(this);
  internal->_object = v8::Null();
  }

XScriptObject::XScriptObject(bool x)
  {
  XScriptObjectInternal *internal = XScriptObjectInternal::init(this);
  internal->_object = v8::Boolean::New(x);
  }

XScriptObject::XScriptObject(xuint32 x)
  {
  XScriptObjectInternal *internal = XScriptObjectInternal::init(this);
  internal->_object = v8::Integer::New(x);
  }

XScriptObject::XScriptObject(xint32 x)
  {
  XScriptObjectInternal *internal = XScriptObjectInternal::init(this);
  internal->_object = v8::Integer::New(x);
  }

XScriptObject::XScriptObject(xuint64 x)
  {
  XScriptObjectInternal *internal = XScriptObjectInternal::init(this);
  internal->_object = v8::Number::New(x);
  }

XScriptObject::XScriptObject(xint64 x)
  {
  XScriptObjectInternal *internal = XScriptObjectInternal::init(this);
  internal->_object = v8::Number::New(x);
  }

XScriptObject::XScriptObject(double x)
  {
  XScriptObjectInternal *internal = XScriptObjectInternal::init(this);
  internal->_object = v8::Number::New(x);
  }

XScriptObject::XScriptObject(float x)
  {
  XScriptObjectInternal *internal = XScriptObjectInternal::init(this);
  internal->_object = v8::Number::New(x);
  }

XScriptObject::XScriptObject(const QString &str)
  {
  XScriptObjectInternal *internal = XScriptObjectInternal::init(this);

  if(str.length())
    {
    internal->_object = v8::String::New((const uint16_t*)str.constData(), str.length());
    }
  else
    {
    internal->_object = v8::String::New("");
    }
  }

XScriptObject::XScriptObject(const XInterfaceObject &obj)
  {
  XScriptObjectInternal *internal = XScriptObjectInternal::init(this);
  internal->_object = getV8Internal(obj);
  }

XScriptObject::~XScriptObject()
  {
  XScriptObjectInternal::term(this);
  }

XScriptObject::XScriptObject(const XScriptObject &other)
  {
  XScriptObjectInternal::init(this, &other);
  }

XScriptObject& XScriptObject::operator=(const XScriptObject &other)
  {
  XScriptObjectInternal::term(this);
  XScriptObjectInternal::init(this, &other);
  return *this;
  }

void XScriptObject::clear()
  {
  XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  internal->_object.Clear();
  }

bool XScriptObject::isValid() const
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  return !internal->_object.IsEmpty() &&
      !internal->_object->IsNull() &&
      !internal->_object->IsUndefined();
  }

bool XScriptObject::isObject() const
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  return internal->_object->IsObject();
  }

void *XScriptObject::toExternal() const
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  return internal->_object.As<v8::External>()->Value();
  }

double XScriptObject::toNumber() const
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  return internal->_object.As<v8::Number>()->Value();
  }

xint64 XScriptObject::toInteger() const
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  return internal->_object.As<v8::Integer>()->Value();
  }

bool XScriptObject::toBoolean() const
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  return internal->_object->ToBoolean()->Value();
  }

QString XScriptObject::toString() const
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  v8::Handle<v8::String> str =  internal->_object.As<v8::String>();

  QString strOut;
  strOut.resize(str->Length());

  xCompileTimeAssert(sizeof(QChar) == sizeof(uint16_t));
  str->Write((uint16_t*)strOut.data(), 0, strOut.length());
  return strOut;
  }

XScriptObject fromHandle(v8::Handle<v8::Value> v)
  {
  XScriptObject o;
  XScriptObjectInternal *internal = XScriptObjectInternal::val(&o);
  internal->_object = v;
  return o;
  }

v8::Handle<v8::Value> getV8Internal(const XScriptObject &o)
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(&o);
  return internal->_object;
  }

void XPersistentScriptObject::dispose()
  {
  const XScriptObjectInternal *internal = XScriptObjectInternal::val(this);
  v8::Persistent<v8::Value>* val = (v8::Persistent<v8::Value>*)&internal->_object;
  val->Dispose();
  }
