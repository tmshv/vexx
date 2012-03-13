#ifndef XCONVERTFROMSCRIPT_H
#define XCONVERTFROMSCRIPT_H

#include "XAssert"
#include "XConvert.h"
#include "XScriptTypeInfo.h"
#include "XSignatureHelpers.h"
#include "XSignatureSpecialisations.h"

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

  ResultType operator()(XScriptObject const &h) const
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

  ResultType operator()(XScriptObject const &h) const
    {
    typedef JSToNative<JST &> Cast;
    typedef typename Cast::ResultType NH;
    return Cast()(h);
    }
  };

template <typename JST> struct JSToNativeAbstract
  {
  typedef typename XScriptTypeInfo<JST>::NativeHandle ResultType;

  ResultType operator()( XScriptObject const & ) const
    {
    return 0;
    }
  };

//template <typename T> struct JSToNative<XScriptObject>
//  {
//  typedef v8::Handle<T> ResultType;

//  ResultType operator()( XScriptObject const & h ) const
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
template <> struct JSToNative<void>
  {
  typedef void ResultType;

  ResultType operator()(...) const
    {
    return;
    }
  };

template <typename T,
          int InternalFieldCount = 1,
          int InternalFieldIndex = 0,
          bool SearchPrototypeChain = false>
struct JSToNativeObjectWithInternalFields
  {
public:
  typedef typename XScriptTypeInfo<T>::NativeHandle ResultType;

  ResultType operator()(v8::Handle<v8::Value> const &h) const
    {
    if( h.IsEmpty() || ! h->IsObject() )
      {
      return 0;
      }
    else
      {
      void *ext = 0;
      v8::Handle<v8::Value> proto(h);
      while(!ext && !proto.IsEmpty() && proto->IsObject())
        {
        v8::Local<v8::Object> const & obj(v8::Object::Cast(*proto));
        ext = (obj->InternalFieldCount() != InternalFieldCount)
          ? 0
          : obj->GetPointerFromInternalField(InternalFieldIndex);
        if(!ext)
          {
          if(!SearchPrototypeChain)
            {
            break;
            }
          else
            {
            proto = obj->GetPrototype();
            }
          }
        }
      return ext ? static_cast<ResultType>(ext) : 0;
      }
    }

#ifdef X_ASSERTS_ENABLED
private:
  typedef char AssertIndexRanges
    [xCompileTimeAssertDef<
    (InternalFieldIndex>=0)
    && (InternalFieldCount>0)
    && (InternalFieldIndex < InternalFieldCount)
    >::Value
    ? 1 : -1];
#endif
  };

template <typename T,
          void const *&TypeID,
          int InternalFieldCount = 2,
          int TypeIdFieldIndex = 0,
          int ObjectFieldIndex = 1,
          bool SearchPrototypeChain = false>
struct JSToNativeObjectWithInternalFieldsTypeSafe
  {
public:
  typedef typename XScriptTypeInfo<T>::NativeHandle ResultType;

  ResultType operator()(v8::Handle<v8::Value> const &h) const
    {
    if(h.IsEmpty() || ! h->IsObject())
      {
      return 0;
      }
    else
      {
      void const *tid = 0;
      void *ext = 0;
      v8::Handle<v8::Value> proto(h);
      while(!ext && !proto.IsEmpty() && proto->IsObject())
        {
        v8::Local<v8::Object> const & obj(v8::Object::Cast(*proto));
        tid = (obj->InternalFieldCount() != InternalFieldCount)
          ? 0
          : obj->GetPointerFromInternalField(TypeIdFieldIndex);
        ext = (tid == TypeID)
          ? obj->GetPointerFromInternalField(ObjectFieldIndex)
          : 0;
        if(!ext)
          {
          if(!SearchPrototypeChain)
            {
            break;
            }
          else
            {
            proto = obj->GetPrototype();
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

namespace
{
template <typename Ret, Ret (XScriptObject::*ToA)() const> struct JSToNativePODType
  {
  typedef Ret ResultType;

  ResultType operator()(XScriptObject const &h) const
    {
    return ((*h)->*ToA)();
    }
  };

template <typename ExtType> struct JSToNativeExternalType
    : JSToNativePODType<ExtType, &XScriptObject::toExternal>
  {
  };

template <typename IntType> struct JSToNativeIntegerType
    : JSToNativePODType<IntType, &XScriptObject::toInteger>
  {
  };

template <typename NumType> struct JSToNativeNumberType
    : JSToNativePODType<NumType, &XScriptObject::toNumber>
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

  ResultType operator()(v8::Handle<v8::Value> const &h) const
    {
    return h->BooleanValue();
    }
  };

template <> struct JSToNative<QString>
  {
  typedef QString ResultType;

  ResultType operator()(XScriptObject const &h) const
    {
    return h.toString();
    }
  };

namespace
{
template <typename T> struct UselessConversionTypeToNative
  {
  };
}

template <> struct JSToNative< XIfElse<
    XSameType<unsigned long int,uint64_t>::Value,
    UselessConversionTypeToNative<unsigned long>,
    unsigned long >::Type >
    : JSToNative<uint64_t>
  {
  };

template <> struct JSToNative< XIfElse<
    XSameType<long,int64_t>::Value,
    UselessConversionTypeToNative<long>,
    long >::Type > : JSToNative<int64_t>
  {
  };

#if X_HAS_LONG_LONG
template <> struct NativeToJS< XIfElse< tmp::SameType<long long int,int64_t>::Value,
    Detail::UselessConversionType<long long>,
    long long int >::Type >
    : Detail::NativeToJS_int_big<int64_t>
  {
  };
#endif

}

template <typename NT> typename internal::JSToNative<NT>::ResultType from(XScriptObject const &h)
  {
  typedef internal::JSToNative<NT> F;
  return F()( h );
  }

}
#endif // XCONVERTFROMSCRIPT_H
