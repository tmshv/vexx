#include "XScriptValue.h"
#include "XScriptValueV8Internals.h"
#include "XScriptObject.h"
#include "XUnorderedMap"
#include "XAssert"

struct XScriptValueInternal
  {
  static XScriptValueInternal *init(XScriptValue *o, const XScriptValue *other=0)
    {
    XScriptValueInternal *internal = (XScriptValueInternal*)o;
    new(internal) XScriptValueInternal;

    if(other)
      {
      const XScriptValueInternal *otherInternal = (const XScriptValueInternal*)other;
      internal->_object = otherInternal->_object;
      }

    return internal;
    }

  static void term(XScriptValue *o)
    {
    XScriptValueInternal *internal = (XScriptValueInternal*)o;
    (void)internal;
    internal->~XScriptValueInternal();
    }

  static const XScriptValueInternal *val(const XScriptValue *o)
    {
    const XScriptValueInternal *internal = (XScriptValueInternal*)o;
    return internal;
    }

  static XScriptValueInternal *val(XScriptValue *o)
    {
    XScriptValueInternal *internal = (XScriptValueInternal*)o;
    return internal;
    }

  mutable v8::Handle<v8::Value> _object;
  };
xCompileTimeAssert(sizeof(XScriptValue) == sizeof(XScriptValueInternal));

XScriptValue::XScriptValue()
  {
  XScriptValueInternal *internal = XScriptValueInternal::init(this);
  internal->_object = v8::Null();
  }

XScriptValue::XScriptValue(bool x)
  {
  XScriptValueInternal *internal = XScriptValueInternal::init(this);
  internal->_object = v8::Boolean::New(x);
  }

XScriptValue::XScriptValue(xuint32 x)
  {
  XScriptValueInternal *internal = XScriptValueInternal::init(this);
  internal->_object = v8::Integer::New(x);
  }

XScriptValue::XScriptValue(xint32 x)
  {
  XScriptValueInternal *internal = XScriptValueInternal::init(this);
  internal->_object = v8::Integer::New(x);
  }

XScriptValue::XScriptValue(xuint64 x)
  {
  XScriptValueInternal *internal = XScriptValueInternal::init(this);
  internal->_object = v8::Number::New(x);
  }

XScriptValue::XScriptValue(xint64 x)
  {
  XScriptValueInternal *internal = XScriptValueInternal::init(this);
  internal->_object = v8::Number::New(x);
  }

XScriptValue::XScriptValue(double x)
  {
  XScriptValueInternal *internal = XScriptValueInternal::init(this);
  internal->_object = v8::Number::New(x);
  }

XScriptValue::XScriptValue(float x)
  {
  XScriptValueInternal *internal = XScriptValueInternal::init(this);
  internal->_object = v8::Number::New(x);
  }

XScriptValue::XScriptValue(const QString &str)
  {
  XScriptValueInternal *internal = XScriptValueInternal::init(this);

  if(str.length())
    {
    internal->_object = v8::String::New((const uint16_t*)str.constData(), str.length());
    }
  else
    {
    internal->_object = v8::String::New("");
    }
  }

XScriptValue::XScriptValue(const XScriptObject &obj)
  {
  XScriptValueInternal *internal = XScriptValueInternal::init(this);
  internal->_object = getV8Internal(obj);
  }

XScriptValue::XScriptValue(const QVariant& val)
  {
  XScriptValueInternal::init(this);

  switch(val.type())
    {
    case QVariant::String:
      *this = XScriptValue(val.toString());
      break;
    case QVariant::Bool:
      *this = XScriptValue(val.toBool());
      break;
    case QVariant::Int:
      *this = XScriptValue(val.toInt());
      break;
    case QVariant::Double:
      *this = XScriptValue(val.toDouble());
      break;

    default:
      {
      const XInterfaceBase* interface = findInterface(val.userType());
      xAssert(interface);
      if(interface)
        {
        *this = interface->copyValue(val);
        }
      break;
      }
    }
  }

XScriptValue::XScriptValue(void* val)
  {
  XScriptValueInternal *internal = XScriptValueInternal::init(this);
  internal->_object = v8::External::New(val);
  }

XScriptValue::~XScriptValue()
  {
  XScriptValueInternal::term(this);
  }

XScriptValue::XScriptValue(const XScriptValue &other)
  {
  XScriptValueInternal::init(this, &other);
  }

XScriptValue& XScriptValue::operator=(const XScriptValue &other)
  {
  XScriptValueInternal::term(this);
  XScriptValueInternal::init(this, &other);
  return *this;
  }

void XScriptValue::clear()
  {
  XScriptValueInternal *internal = XScriptValueInternal::val(this);
  internal->_object.Clear();
  }

bool XScriptValue::isValid() const
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(this);
  return !internal->_object.IsEmpty() &&
      !internal->_object->IsNull() &&
      !internal->_object->IsUndefined();
  }

bool XScriptValue::isObject() const
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(this);
  return internal->_object->IsObject();
  }

bool XScriptValue::isArray() const
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(this);
  return internal->_object->IsArray();
  }

xsize XScriptValue::length() const
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(this);
  v8::Handle<v8::Array> arr = internal->_object.As<v8::Array>();
  return arr->Length();
  }

XScriptValue XScriptValue::at(xsize id)
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(this);
  v8::Handle<v8::Array> arr = internal->_object.As<v8::Array>();
  return fromHandle(arr->Get(id));
  }

void *XScriptValue::toExternal() const
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(this);
  return internal->_object.As<v8::External>()->Value();
  }

double XScriptValue::toNumber() const
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(this);
  return internal->_object.As<v8::Number>()->Value();
  }

xint64 XScriptValue::toInteger() const
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(this);
  if(internal->_object->IsNumber())
    {
    return internal->_object.As<v8::Number>()->Value();
    }
  return 0;
  }

bool XScriptValue::toBoolean() const
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(this);
  if(internal->_object->IsBoolean())
    {
    return internal->_object->ToBoolean()->Value();
    }
  return false;
  }

QString XScriptValue::toString() const
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(this);
  v8::Handle<v8::String> str = internal->_object->ToString();

  QString strOut;
  strOut.resize(str->Length());

  xCompileTimeAssert(sizeof(QChar) == sizeof(uint16_t));
  str->Write((uint16_t*)strOut.data(), 0, strOut.length());
  return strOut;
  }

QVariant XScriptValue::toVariant(int typeHint) const
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(this);
  if(typeHint == QVariant::String || internal->_object->IsString())
    {
    return toString();
    }

  if(internal->_object->IsObject())
    {
    XScriptObject object(*this);
    XInterfaceBase* interface = object.getInterface();
    if(interface)
      {
      return interface->toVariant(*this, typeHint);
      }
    }

  xAssertFail();
  return QVariant();
  }

XScriptValue fromHandle(v8::Handle<v8::Value> v)
  {
  XScriptValue o;
  XScriptValueInternal *internal = XScriptValueInternal::val(&o);
  internal->_object = v;
  return o;
  }

v8::Handle<v8::Value> getV8Internal(const XScriptValue &o)
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(&o);
  return internal->_object;
  }

void XPersistentScriptValue::dispose()
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(this);
  v8::Persistent<v8::Value>* val = (v8::Persistent<v8::Value>*)&internal->_object;
  val->Dispose();
  }
