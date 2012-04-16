#ifndef XCONVERTFROMSCRIPT_H
#define XCONVERTFROMSCRIPT_H

#include "XAssert"
#include "XConvert.h"
#include "XScriptTypeInfo.h"
#include "XSignatureHelpers.h"
#include "XSignatureSpecialisations.h"
#include "XInterface.h"

namespace XScriptConvert
{

namespace internal
{

template <typename JST> struct JSToNative<JST const> : JSToNative<JST> {};
template <typename JST> struct JSToNative<JST *> : JSToNative<JST> {};
template <typename JST> struct JSToNative<JST const *> : JSToNative<JST> {};

template <typename JST> struct JSToNative<JST &>
  {
  typedef typename XScriptTypeInfo<JST>::Type &ResultType;

  ResultType operator()(XScriptValue const &h) const
    {
    typedef JSToNative<JST*> Cast;
    typedef typename Cast::ResultType NH;
    NH n = Cast()( h );
    if( ! n )
      {
      throw std::runtime_error("JSToNative<T&> could not get native pointer. Throwing to avoid dereferencing null!");
      }
    else
      {
      return *n;
      }
    }
  };

template <typename JST> struct JSToNative<JST const &>
  {
  typedef typename XScriptTypeInfo<JST>::Type const &ResultType;

  ResultType operator()(XScriptValue const &h) const
    {
    typedef JSToNative<JST &> Cast;
    typedef typename Cast::ResultType NH;
    return Cast()(h);
    }
  };

template <typename JST> struct JSToNativeAbstract
  {
  typedef typename XScriptTypeInfo<JST>::NativeHandle ResultType;

  ResultType operator()( XScriptValue const & ) const
    {
    return 0;
    }
  };

//template <typename T> struct JSToNative<XScriptValue>
//  {
//  typedef v8::Handle<T> ResultType;

//  ResultType operator()( XScriptValue const & h ) const
//    {
//    return h;
//    }
//  };

//template <typename T> struct JSToNative<v8::Handle<T> const &> : JSToNative< v8::Handle<T> > {};
//template <typename T> struct JSToNative<v8::Handle<T> &> : JSToNative< v8::Handle<T> > {};

//template <typename T> struct JSToNative<v8::Local<T> >
//  {
//  typedef v8::Local<T> ResultType;

//  ResultType operator()( v8::Local<T> const & h ) const
//    {
//    return h;
//    }
//  };

//template <typename T> struct JSToNative<v8::Local<T> const &> : JSToNative< v8::Local<T> > {};
//template <typename T> struct JSToNative<v8::Local<T> &> : JSToNative< v8::Local<T> > {};
/*
namespace
{
template <typename V8Type, bool (v8::Value::*IsA)() const> struct JSToNativeV8Type
  {
  typedef v8::Handle<V8Type> ResultType;

  ResultType operator()(v8::Handle<v8::Value> const &h) const
    {
    return (h.IsEmpty() || !((*h)->*IsA)())
        ? v8::Handle<V8Type>()
        : v8::Handle<V8Type>(V8Type::Cast(*h));
    }
  };
}

template <> struct JSToNative<v8::Handle<v8::Object> > : JSToNativeV8Type<v8::Object, &v8::Value::IsObject> {};
template <> struct JSToNative<v8::Handle<v8::Object> &> : JSToNative<v8::Handle<v8::Object> > {};
template <> struct JSToNative<v8::Handle<v8::Object> const &> : JSToNative<v8::Handle<v8::Object> > {};

template <> struct JSToNative<v8::Handle<v8::Array> > : JSToNativeV8Type<v8::Array, &v8::Value::IsArray> {};
template <> struct JSToNative<v8::Handle<v8::Array> &> : JSToNative<v8::Handle<v8::Array> > {};
template <> struct JSToNative<v8::Handle<v8::Array> const &> : JSToNative<v8::Handle<v8::Array> > {};

template <> struct JSToNative<v8::Handle<v8::Function> > : JSToNativeV8Type<v8::Function, &v8::Value::IsFunction> {};
template <> struct JSToNative<v8::Handle<v8::Function> &> : JSToNative<v8::Handle<v8::Function> > {};
template <> struct JSToNative<v8::Handle<v8::Function> const &> : JSToNative<v8::Handle<v8::Function> > {};
*/

template <> struct JSToNative<XScriptFunction>
  {
  typedef XScriptFunction ResultType;

  ResultType operator()(XScriptValue const &h) const
    {
    return XScriptFunction(h);
    }
  };
template <> struct JSToNative<XScriptFunction const &> : JSToNative<XScriptFunction> {};

template <> struct JSToNative<void>
  {
  typedef void ResultType;

  ResultType operator()(...) const
    {
    return;
    }
  };

//template <typename T,
//          int InternalFieldCount = 1,
//          int InternalFieldIndex = 0,
//          bool SearchPrototypeChain = false>
//struct JSToNativeObjectWithInternalFields
//  {
//public:
//  typedef typename XScriptTypeInfo<T>::NativeHandle ResultType;

//  ResultType operator()(XScriptValue const &h) const
//    {
//    if( !h.isValid() || ! h.isObject() )
//      {
//      return 0;
//      }
//    else
//      {
//      void *ext = 0;
//      XScriptValue proto(h);
//      while(!ext && proto.isValid() && proto.isObject())
//        {
//        XScriptObject const &obj(proto);
//        ext = (obj.internalFieldCount() != InternalFieldCount)
//          ? 0
//          : obj.internalField(InternalFieldIndex);
//        if(!ext)
//          {
//          if(!SearchPrototypeChain)
//            {
//            break;
//            }
//          else
//            {
//            proto = obj.getPrototype();
//            }
//          }
//        }
//      return ext ? static_cast<ResultType>(ext) : 0;
//      }
//    }

//#ifdef X_ASSERTS_ENABLED
//private:
//  typedef char AssertIndexRanges
//    [xCompileTimeAssertDef<
//    (InternalFieldIndex>=0)
//    && (InternalFieldCount>0)
//    && (InternalFieldIndex < InternalFieldCount)
//    >::Value
//    ? 1 : -1];
//#endif
//  };

template <typename T,
          int InternalFieldCount = XScript::ClassCreator_InternalFields<T>::Count,
          int TypeIdFieldIndex = XScript::ClassCreator_InternalFields<T>::TypeIDIndex,
          int ObjectFieldIndex = XScript::ClassCreator_InternalFields<T>::NativeIndex,
          bool SearchPrototypeChain = XScript::ClassCreator_SearchPrototypeForThis<T>::Value>
struct JSToNativeObject
  {
public:
  typedef typename XScriptTypeInfo<T>::NativeHandle ResultType;

  ResultType operator()(XScriptValue const &h) const
    {
    if(!h.isObject())
      {
      return 0;
      }
    else
      {
      const xsize TypeID = findInterface<T>((const T*)0)->typeId();
      void const *tid = 0;
      void *ext = 0;
      XScriptValue proto(h);
      while(!ext && proto.isObject())
        {
        XScriptObject const &obj(proto);
        tid = (obj.internalFieldCount() != InternalFieldCount)
          ? 0
          : obj.internalField(TypeIdFieldIndex);
        ext = ((xsize)tid == TypeID)
          ? obj.internalField(ObjectFieldIndex)
          : 0;
        if(!ext)
          {
          if(!SearchPrototypeChain)
            {
            break;
            }
          else
            {
            proto = obj.getPrototype();
            }
          }
        }
      return ext ? static_cast<ResultType>(ext) : 0;
      }
    }

private:
  typedef char AssertIndexRanges
    [(InternalFieldCount>=2)
    && (TypeIdFieldIndex != ObjectFieldIndex)
    && (TypeIdFieldIndex >= 0)
    && (TypeIdFieldIndex < InternalFieldCount)
    && (ObjectFieldIndex >= 0)
    && (ObjectFieldIndex < InternalFieldCount)
    ? 1 : -1];
  };



template <typename T, typename BASE,
          int InternalFieldCount = XScript::ClassCreator_InternalFields<T>::Count,
          int TypeIdFieldIndex = XScript::ClassCreator_InternalFields<T>::TypeIDIndex,
          int ObjectFieldIndex = XScript::ClassCreator_InternalFields<T>::NativeIndex,
          bool SearchPrototypeChain = XScript::ClassCreator_SearchPrototypeForThis<T>::Value>
struct JSToNativeObjectInherited
  {
public:
  typedef typename XScriptTypeInfo<T>::NativeHandle ResultType;

  ResultType operator()(XScriptValue const &h) const
    {
    if(!h.isObject())
      {
      return 0;
      }
    else
      {
      const xsize TypeID = findInterface<BASE>((const BASE*)0)->typeId();
      void const *tid = 0;
      void *ext = 0;
      XScriptValue proto(h);
      while(!ext && proto.isObject())
        {
        XScriptObject const &obj(proto);
        tid = (obj.internalFieldCount() != InternalFieldCount)
          ? 0
          : obj.internalField(TypeIdFieldIndex);
        ext = ((xsize)tid == TypeID)
          ? obj.internalField(ObjectFieldIndex)
          : 0;
        if(!ext)
          {
          if(!SearchPrototypeChain)
            {
            break;
            }
          else
            {
            proto = obj.getPrototype();
            }
          }
        }
      return ext ? XScriptConvert::castFromBase<T>(static_cast<BASE*>(ext)) : 0;
      }
    }

private:
  typedef char AssertIndexRanges
    [(InternalFieldCount>=2)
    && (TypeIdFieldIndex != ObjectFieldIndex)
    && (TypeIdFieldIndex >= 0)
    && (TypeIdFieldIndex < InternalFieldCount)
    && (ObjectFieldIndex >= 0)
    && (ObjectFieldIndex < InternalFieldCount)
    ? 1 : -1];
  };

namespace
{
template <typename Ret, Ret (XScriptValue::*ToA)() const> struct JSToNativePODType
  {
  typedef Ret ResultType;

  ResultType operator()(XScriptValue const &h) const
    {
    return (h.*ToA)();
    }
  };

template <typename ExtType> struct JSToNativeExternalType
    : JSToNativePODType<ExtType, &XScriptValue::toExternal>
  {
  };

template <typename IntType> struct JSToNativeIntegerType
    : JSToNativePODType<IntType, &XScriptValue::toInteger>
  {
  };

template <typename NumType> struct JSToNativeNumberType
    : JSToNativePODType<NumType, &XScriptValue::toNumber>
  {
  };
}

template <> struct JSToNative<void *> : JSToNativeExternalType<void *> {};

template <> struct JSToNative<xint8> : JSToNativeIntegerType<xint64> {};
template <> struct JSToNative<xuint8> : JSToNativeIntegerType<xint64> {};
template <> struct JSToNative<xint16> : JSToNativeIntegerType<xint64> {};
template <> struct JSToNative<xuint16> : JSToNativeIntegerType<xint64> {};
template <> struct JSToNative<xint32> : JSToNativeIntegerType<xint64> {};
template <> struct JSToNative<xuint32> : JSToNativeIntegerType<xint64> {};
template <> struct JSToNative<xint64> : JSToNativeIntegerType<xint64> {};
template <> struct JSToNative<xuint64> : JSToNativeIntegerType<xint64> {};

template <> struct JSToNative<double> : JSToNativeNumberType<double> {};
template <> struct JSToNative<float> : JSToNativeNumberType<double> {};

template <> struct JSToNative<bool>
  {
  typedef bool ResultType;

  ResultType operator()(XScriptValue const &h) const
    {
    return h.toBoolean();
    }
  };

template <> struct JSToNative<QString>
  {
  typedef QString ResultType;

  ResultType operator()(XScriptValue const &h) const
    {
    return h.toString();
    }
  };

template <> struct JSToNative<const QString &> : JSToNative<QString>
  {
  };

template <> struct JSToNative<QByteArray>
  {
  typedef QByteArray ResultType;

  ResultType operator()(XScriptValue const &h) const
    {
    return h.toString().toUtf8();
    }
  };

template <> struct JSToNative<const QByteArray &> : JSToNative<QByteArray>
  {
  };

template <> struct JSToNative<QVariant>
  {
  typedef QVariant ResultType;

  ResultType operator()(XScriptValue const &h) const
    {
    return h.toVariant();
    }
  };

template <> struct JSToNative<const QVariant &> : JSToNative<QVariant>
  {
  };

namespace
{
template <typename T> struct UselessConversionTypeToNative
  {
  };
}

template <> struct JSToNative< XIfElse<
    XSameType<unsigned long int,xuint64>::Value,
    UselessConversionTypeToNative<unsigned long>,
    unsigned long >::Type >
    : JSToNative<xuint64>
  {
  };

template <> struct JSToNative< XIfElse<
    XSameType<long,xint64>::Value,
    UselessConversionTypeToNative<long>,
    long >::Type > : JSToNative<xint64>
  {
  };
}

template <typename NT> typename internal::JSToNative<NT>::ResultType from(XScriptValue const &h)
  {
  typedef internal::JSToNative<NT> F;
  return F()( h );
  }

}

#endif // XCONVERTFROMSCRIPT_H
