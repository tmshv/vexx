#include "XQObjectWrapper.h"
#include "QMetaProperty"
#include "XScriptValueV8Internals.h"
#include "QVarLengthArray"

Q_DECLARE_METATYPE(QObjectList)

XQObjectWrapper::XQObjectWrapper()
  {
  _context = 0;
  }

XQObjectWrapper::~XQObjectWrapper()
  {
  foreach(XInterfaceBase *b, _objects)
    {
    if(b != XInterface<QObject>::lookup())
      {
      delete b;
      }
    }
  _objects.clear();
  }

QString formatClassName(const QString &n)
  {
  if(n[0] == 'Q')
    {
    return n.right(n.length()-1);
    }
  return n;
  }

const char *qobjectName = "QObject";
void XQObjectWrapper::initiate(XContext *c)
  {
  instance()->_context = c;

  XInterface<QObject>* interface = XInterface<QObject>::create(qobjectName, 0);
  buildInterface(interface, &QObject::staticMetaObject);
  interface->seal();
  c->addInterface(interface);
  instance()->_objects.insert(&QObject::staticMetaObject, interface);
  }

XQObjectWrapper *XQObjectWrapper::instance()
  {
  static XQObjectWrapper wrap;
  return &wrap;
  }

XScriptObject XQObjectWrapper::wrap(QObject *obj)
  {
  const XInterfaceBase* interface = findInterface(obj->metaObject());

  XScriptObject self = interface->newInstanceBase();
  interface->wrapInstance(self, obj);

  return self;
  }

XInterfaceBase *XQObjectWrapper::findInterface(const QMetaObject *object)
  {
  XInterfaceBase *base = _objects[object];
  if(base)
    {
    return base;
    }

  base = new XInterface<QObject>((xsize)qobjectName, formatClassName(object->className()));

  const QMetaObject *parent = object->superClass();
  if(parent)
    {
    XInterfaceBase *parentInterface = findInterface(parent);
    base->inherit(parentInterface);
    }

  _objects.insert(object, base);

  buildInterface(base, object);

  base->seal();
  _context->addInterface(base);

  return base;
  }

template<typename A, typename B, typename C, typename D, typename E>
class MaxSizeOf5
  {
  template<typename Z, typename X> struct SMax {
    static const xsize Size = sizeof(Z) > sizeof(X) ? sizeof(Z) : sizeof(X);
    };
public:

  static const size_t Size = SMax<A, SMax<B, SMax<C, SMax<D, E> > > >::Size;
  };

struct CallArgument
  {
  inline CallArgument()
  : type(QVariant::Invalid)
  {
  }
  inline ~CallArgument()
  {
      cleanup();
  }
  inline void *dataPtr()
  {
      if (type == -1)
          return qvariantPtr->data();
      else
          return (void *)&allocData;
  }

  void CallArgument::initAsType(int callType)
    {
    if (type != 0) { cleanup(); type = 0; }
    if (callType == 0) return;

    /*if (callType == qMetaTpeId<QJSValue>())
      {
      qjsValuPtr = new (&allocData) QJSValue();
      type = callType;
      }
    else */if (callType == QMetaType::Int ||
                 callType == QMetaType::UInt ||
                 callType == QMetaType::Bool ||
                 callType == QMetaType::Double ||
                 callType == QMetaType::Float)
      {
      type = callType;
      }
    else if (callType == QMetaType::QObjectStar)
      {
      qobjectPtr = 0;
      type = callType;
      }
    else if (callType == QMetaType::QString)
      {
      qstringPtr = new (&allocData) QString();
      type = callType;
      }
    else if (callType == QMetaType::QVariant)
      {
      type = callType;
      qvariantPtr = new (&allocData) QVariant();
      }
    else if (callType == qMetaTypeId<QList<QObject *> >())
      {
      type = callType;
      qlistPtr = new (&allocData) QList<QObject *>();
      }
    /*else if (callType == qMetaTypeId<QQmlV8Handle>())
      {
      type = callType;
      handlePtr = new (&allocData) QQmlV8Handle;
      }*/
    else
      {
      type = -1;
      qvariantPtr = new (&allocData) QVariant(callType, (void *)0);
      }
    }

    void CallArgument::fromValue(int callType, v8::Handle<v8::Value> value)
      {
      if (type != 0)
        {
        cleanup();
        type = 0;
        }

      /*if (callType == qMetaTypeId<QJSValue>()) {
        qjsValuePtr = new (&allocData) QJSValue(QJSValuePrivate::get(new QJSValuePrivate(engine, value)));
        type = qMetaTypeId<QJSValue>();
        } else */
      if (callType == QMetaType::Int)
        {
        intValue = quint32(value->Int32Value());
        type = callType;
        }
      else if (callType == QMetaType::UInt)
        {
        intValue = quint32(value->Uint32Value());
        type = callType;
        }
      else if (callType == QMetaType::Bool)
        {
        boolValue = value->BooleanValue();
        type = callType;
        }
      else if (callType == QMetaType::Double)
        {
        doubleValue = double(value->NumberValue());
        type = callType;
        }
      else if (callType == QMetaType::Float)
        {
        floatValue = float(value->NumberValue());
        type = callType;
        }
      else if (callType == QMetaType::QString)
        {
        if (value->IsNull() || value->IsUndefined())
          {
          qstringPtr = new (&allocData) QString();
          }
        else
          {
          qstringPtr = new (&allocData) QString(XScriptConvert::from<QString>(fromHandle(value)));
          }
        type = callType;
        }
      else if (callType == QMetaType::QObjectStar)
        {
        qobjectPtr = XScriptConvert::from<QObject>(fromHandle(value));
        type = callType;
        }
      else if (callType == qMetaTypeId<QVariant>())
        {
        qvariantPtr = new (&allocData) QVariant(fromHandle(value).toVariant());
        type = callType;
        }
      else if (callType == qMetaTypeId<QList<QObject*> >())
        {
        qlistPtr = new (&allocData) QList<QObject *>();
        if (value->IsArray())
          {
          v8::Handle<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
          uint32_t length = array->Length();
          for (uint32_t ii = 0; ii < length; ++ii)
            {
            qlistPtr->append(XScriptConvert::from<QObject>(fromHandle(array->Get(ii))));
            }
          }
        else
          {
          qlistPtr->append(XScriptConvert::from<QObject>(fromHandle(value)));
          }
        type = callType;
        }
      /*else if (callType == qMetaTypeId<QQmlV8Handle>())
        {
        handlePtr = new (&allocData) QQmlV8Handle(QQmlV8Handle::fromHandle(value));
        type = callType;
        }*/
      else
        {
        qvariantPtr = new (&allocData) QVariant();
        type = -1;

        QVariant v = fromHandle(value).toVariant();
        if (v.userType() == callType)
          {
          *qvariantPtr = v;
          }
        else if (v.canConvert((QVariant::Type)callType))
          {
          *qvariantPtr = v;
          qvariantPtr->convert((QVariant::Type)callType);
          }
        else /*if (const QMetaObject *mo = ep ? ep->rawMetaObjectForType(callType) : 0)
          {
          QObject *obj = ep->toQObject(v);

          if (obj)
            {
            const QMetaObject *objMo = obj->metaObject();
            while (objMo && objMo != mo) objMo = objMo->superClass();
            if (!objMo) obj = 0;
            }

          *qvariantPtr = QVariant(callType, &obj);
          }
        else*/
          {
          *qvariantPtr = QVariant(callType, (void *)0);
          }
        }
      }

  v8::Handle<v8::Value> CallArgument::toValue()
    {
    /*if (type == qMetaTypeId<QJSValue>())
      {
      return QJSValuePrivate::get(*qjsValuePtr)->asV8Value(engine);
      }
    else */if (type == QMetaType::Int)
      {
      return v8::Integer::New(int(intValue));
      }
    else if (type == QMetaType::UInt)
      {
      return v8::Integer::NewFromUnsigned(intValue);
      }
    else if (type == QMetaType::Bool)
      {
      return v8::Boolean::New(boolValue);
      }
    else if (type == QMetaType::Double)
      {
      return v8::Number::New(doubleValue);
      }
    else if (type == QMetaType::Float)
      {
      return v8::Number::New(floatValue);
      }
    else if (type == QMetaType::QString)
      {
      return getV8Internal(XScriptConvert::to(*qstringPtr));
      }
    else if (type == QMetaType::QObjectStar)
      {
      return getV8Internal(XScriptConvert::to(qobjectPtr));
      }
    else if (type == qMetaTypeId<QList<QObject *> >())
      {
      // XXX Can this be made more by using Array as a prototype and implementing
      // directly against QList<QObject*>?
      QList<QObject *> &list = *qlistPtr;
      v8::Local<v8::Array> array = v8::Array::New(list.count());
      for (int ii = 0; ii < list.count(); ++ii)
        {
        array->Set(ii, getV8Internal(XScriptConvert::to(list.at(ii))));
        }
      return array;
      }/*
    else if(type == qMetaTypeId<QQmlV8Handle>())
      {
      return handlePtr->toHandle();
      }*/
    else if (type == -1 || type == qMetaTypeId<QVariant>())
      {
      return getV8Internal(XScriptValue(*qvariantPtr));
      }
    else
      {
      return v8::Undefined();
      }
    }

private:
  CallArgument(const CallArgument &);
  inline void cleanup()
    {
    if (type == QMetaType::QString)
      {
      qstringPtr->~QString();
      }
    else if (type == -1 || type == QMetaType::QVariant)
      {
      qvariantPtr->~QVariant();
      }/*
    else if (type == qMetaTypeId<QJSValue>())
      {
      qjsValuePtr->~QJSValue();
      }*/
    else if (type == qMetaTypeId<QList<QObject *> >())
      {
      qlistPtr->~QList<QObject *>();
      }
    }


  union {
    float floatValue;
    double doubleValue;
    quint32 intValue;
    bool boolValue;
    QObject *qobjectPtr;

    char allocData[MaxSizeOf5<QVariant,
      QString,
      QList<QObject *>,
      void *,
      void *>::Size];

    qint64 q_for_alignment;
    };

  // Pointers to allocData
  union {
    QString *qstringPtr;
    QVariant *qvariantPtr;
    QList<QObject *> *qlistPtr;
    };

  int type;
};

struct Utils
  {
  static v8::Handle<v8::Value> read(v8::Local<v8::String>, const v8::AccessorInfo& info)
    {
    QObject *ths = XScriptConvert::from<QObject>(fromHandle(info.This()));
    if(!ths)
      {
      return v8::Null();
      }

    int id = XScriptConvert::from<int>(fromHandle(info.Data()));
    QVariant value = ths->metaObject()->property(id).read(ths);
    return getV8Internal(XScriptValue(value));
    }

  static void write(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
    {
    QObject *ths = XScriptConvert::from<QObject>(fromHandle(info.This()));
    if(!ths)
      {
      return;
      }

    int id = XScriptConvert::from<int>(fromHandle(info.Data()));
    ths->metaObject()->property(id).write(ths, fromHandle(value).toVariant());
    }

  static v8::Handle<v8::Value> method(const v8::Arguments& args)
    {
    QObject *ths = XScriptConvert::from<QObject>(fromHandle(args.This()));
    v8::Local<v8::Value> idVal = args.Callee()->Get(0);
    int id = XScriptConvert::from<int>(fromHandle(idVal));

    QMetaMethod method = ths->metaObject()->method(id);
    QList<QByteArray> types = method.parameterTypes();
    int length = args.Length();
    if(length < types.size())
      {
      return getV8Internal(Toss(QString("Too few arguments to method ") + method.signature()));
      }
    length = qMin(length, types.size());

    QVarLengthArray<CallArgument, 9> qargs(length + 1);

    const char *returnTypeStr = method.typeName();
    int returnType = QVariant::Invalid;
    if(returnTypeStr)
      {
      returnType = QMetaType::type(returnTypeStr);
      }

    qargs[0].initAsType(returnType);


    for (int ii = 0; ii < length; ++ii)
      {
      const QByteArray typeStr = types[ii];
      int type = QVariant::Invalid;
      if(typeStr.length())
        {
        type = QMetaType::type(typeStr.constData());
        }

      qargs[ii + 1].fromValue(type, args[ii]);
      }


    QVarLengthArray<void *, 9> argData(qargs.count());
    for (int ii = 0; ii < qargs.count(); ++ii)
      {
      argData[ii] = qargs[ii].dataPtr();
      }


    QMetaObject::metacall(ths, QMetaObject::InvokeMetaMethod, id, argData.data());


    return qargs[0].toValue();
    }
  };

void XQObjectWrapper::buildInterface(XInterfaceBase *interface, const QMetaObject *metaObject)
  {
  v8::Handle<v8::ObjectTemplate> templ = getV8Internal(interface);

  for(int i = metaObject->propertyOffset(), s = metaObject->propertyCount(); i < s; ++i)
    {
    QMetaProperty prop(metaObject->property(i));
    if(!prop.isReadable())
      {
      continue;
      }

    v8::AccessorSetter setter = prop.isWritable() ? Utils::write : 0;

    v8::Handle<v8::Integer> data = v8::Integer::New(i);

    v8::Handle<v8::String> name = v8::String::New(prop.name());
    templ->SetAccessor(name, Utils::read, setter, data);
    }

  for(int i = metaObject->methodOffset(), s = metaObject->methodCount(); i < s; ++i)
    {
    QMetaMethod method(metaObject->method(i));

    if(method.access() != QMetaMethod::Public)
      {
      continue;
      }

    v8::Handle<v8::Integer> id = v8::Integer::New(i);
    v8::Handle<v8::FunctionTemplate> data = v8::FunctionTemplate::New(Utils::method);
    data->Set(v8::String::New("0"), id);

    QString qname = method.signature();
    int firstBrack = qname.indexOf('(');
    qname = qname.left(firstBrack);

    v8::Handle<v8::String> name = v8::String::New((xuint16*)qname.constData(), qname.length());
    templ->Set(name, data);
    }
  }
