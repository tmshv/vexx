#include "XScriptValue.h"
#include "XScriptValueV8Internals.h"
#include "XScriptObject.h"
#include "XUnorderedMap"
#include "QStringList"
#include "XConvertFromScript.h"
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

struct XPersistentScriptValueInternal
  {
  static XPersistentScriptValueInternal *init(XPersistentScriptValue *o, const XPersistentScriptValue *other=0)
    {
    XPersistentScriptValueInternal *internal = (XPersistentScriptValueInternal*)o;
    new(internal) XPersistentScriptValueInternal;

    if(other)
      {
      const XPersistentScriptValueInternal *otherInternal = (const XPersistentScriptValueInternal*)other;
      internal->_object = otherInternal->_object;
      }

    return internal;
    }

  static void term(XPersistentScriptValue *o)
    {
    XPersistentScriptValueInternal *internal = (XPersistentScriptValueInternal*)o;
    (void)internal;
    internal->~XPersistentScriptValueInternal();
    }

  static const XPersistentScriptValueInternal *val(const XPersistentScriptValue *o)
    {
    const XPersistentScriptValueInternal *internal = (XPersistentScriptValueInternal*)o;
    return internal;
    }

  static XPersistentScriptValueInternal *val(XPersistentScriptValue *o)
    {
    XPersistentScriptValueInternal *internal = (XPersistentScriptValueInternal*)o;
    return internal;
    }

  mutable v8::Persistent<v8::Value> _object;
  };
xCompileTimeAssert(sizeof(XPersistentScriptValue) == sizeof(XPersistentScriptValueInternal));

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
  v8::Locker l;
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
    case QVariant::List:
      {
        QVariantList list = val.toList();
        v8::Local<v8::Array> array = v8::Array::New(list.length());
        for(int i = 0, s = list.size(); i < s; ++i)
        {
          array->Set((uint32_t)i, getV8Internal(XScriptValue(list[i])));
        }
        *this = fromHandle(array);
      }
      break;
    case QVariant::StringList:
      {
        QStringList list = val.toStringList();
        v8::Local<v8::Array> array = v8::Array::New(list.length());
        for(int i = 0, s = list.size(); i < s; ++i)
        {
          array->Set((uint32_t)i, getV8Internal(XScriptValue(list[i])));
        }
        *this = fromHandle(array);
      }
      break;
    case QVariant::Map:
      {
      QVariantMap map = val.toMap();
      v8::Local<v8::Object> obj = v8::Object::New();
      QVariantMap::const_iterator it = map.begin();
      QVariantMap::const_iterator end = map.end();
      for(; it != end; ++it)
        {
        QString name = it.key();
        QVariant val = it.value();
        obj->Set(getV8Internal(XScriptConvert::to(name)), getV8Internal(XScriptValue(val)));
        }
      *this = fromHandle(obj);
      }
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


XScriptValue XScriptValue::newArray()
  {
  XScriptValue obj;
  const XScriptValueInternal *internal = XScriptValueInternal::val(&obj);
  internal->_object = v8::Array::New();
  return obj;
  }

XScriptValue XScriptValue::empty()
  {
  XScriptValue obj;
  const XScriptValueInternal *internal = XScriptValueInternal::val(&obj);
  internal->_object = v8::Handle<v8::Value>();
  return obj;
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

void XScriptValue::set(xsize id, const XScriptValue &val)
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(this);
  v8::Handle<v8::Array> arr = internal->_object.As<v8::Array>();
  arr->Set(id, getV8Internal(val));
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
  if(!internal->_object.IsEmpty())
    {
    v8::Handle<v8::String> str = internal->_object->ToString();
    if(!str.IsEmpty())
      {
      QString strOut;
      strOut.resize(str->Length());

      xCompileTimeAssert(sizeof(QChar) == sizeof(uint16_t));
      str->Write((uint16_t*)strOut.data(), 0, strOut.length());
      return strOut;
      }
    else
      {
      //xAssertFail();
      //v8::String::AsciiValue exception_str(internal->_object);
      //return *exception_str;
      }
    }
  return QString();
  }

QVariant XScriptValue::toVariant(int typeHint) const
  {
  const XScriptValueInternal *internal = XScriptValueInternal::val(this);
  if(typeHint == QVariant::Bool || internal->_object->IsBoolean())
    {
    return toBoolean();
    }
  else if(typeHint == QVariant::Int || internal->_object->IsInt32())
    {
    return toInteger();
    }
  else if(typeHint == QVariant::Double || internal->_object->IsNumber())
    {
    return toNumber();
    }
  else if(typeHint == QVariant::String || internal->_object->IsString())
    {
    return toString();
    }
  else if (typeHint == qMetaTypeId<XScriptObject>())
    {
    return QVariant::fromValue(XScriptConvert::from<XScriptObject>(fromHandle(internal->_object)));
    }
  else if (typeHint == qMetaTypeId<XScriptFunction>())
    {
    return QVariant::fromValue(XScriptConvert::from<XScriptFunction>(fromHandle(internal->_object)));
    }
  else if(internal->_object->IsArray())
    {
    v8::Handle<v8::Array> jsArr = internal->_object.As<v8::Array>();
    QVariantList list;
    for(uint32_t i = 0, s = jsArr->Length(); i < s; ++i)
      {
      list << fromHandle(jsArr->Get(i)).toVariant(QVariant::Invalid);
      }
    return list;
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

  XInterfaceBase *interface = typeHint != QVariant::Invalid ? findInterface(typeHint) : 0;
  if(interface)
    {
    return interface->toVariant(XScriptValue(), typeHint);
    }

  if(typeHint == QVariant::Map || internal->_object->IsObject())
    {
    QVariantMap map;
    if(internal->_object->IsObject())
      {
      v8::Handle<v8::Object> obj = internal->_object.As<v8::Object>();
      v8::Handle<v8::Array> propertyNames = obj->GetOwnPropertyNames();
      for(uint32_t i = 0, s = propertyNames->Length(); i < s; ++i)
        {
        v8::Handle<v8::Value> key = propertyNames->Get(i);
        v8::Handle<v8::Value> value = obj->Get(key);

        map.insert(XScriptConvert::from<QString>(fromHandle(key)),
                   fromHandle(value).toVariant(QVariant::Invalid));
        }
      }
    return map;
    }

  xAssert(internal->_object->IsNull());
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

XPersistentScriptValue::XPersistentScriptValue()
  {
  XPersistentScriptValueInternal::init(this);
  }

XPersistentScriptValue::XPersistentScriptValue(const XScriptValue &val)
  {
  const XPersistentScriptValueInternal *internal = XPersistentScriptValueInternal::init(this);

  const XScriptValueInternal *other = XScriptValueInternal::val(&val);
  internal->_object = v8::Persistent<v8::Value>::New(other->_object);
  }

XScriptValue XPersistentScriptValue::asValue() const
  {
  XScriptValue val;
  const XScriptValueInternal *internal = XScriptValueInternal::val(&val);
  const XPersistentScriptValueInternal *other = XPersistentScriptValueInternal::val(this);

  internal->_object = other->_object;
  return val;
  }

void XPersistentScriptValue::makeWeak(void *data, WeakDtor cb)
  {
  const XPersistentScriptValueInternal *internal = XPersistentScriptValueInternal::val(this);
  internal->_object.MakeWeak(data, (v8::WeakReferenceCallback)cb);
  }

void XPersistentScriptValue::dispose()
  {
  const XPersistentScriptValueInternal *internal = XPersistentScriptValueInternal::val(this);
  internal->_object.Dispose();
  internal->_object.Clear();
  }
