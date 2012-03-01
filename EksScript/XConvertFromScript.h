#ifndef XCONVERTFROMSCRIPT_H
#define XCONVERTFROMSCRIPT_H

#include "XAssert"
#include "XConvert.h"
#include "XScriptTypeInfo.h"
#include "XSignatureHelpers.h"
#include "XSignatureSpecialisations.h"

namespace cvv8
{

template <typename JST> struct JSToNative<JST const> : JSToNative<JST> {};
template <typename JST> struct JSToNative<JST *> : JSToNative<JST> {};
template <typename JST> struct JSToNative<JST const *> : JSToNative<JST> {};

template <typename JST> struct JSToNative<JST &>
  {
  typedef typename TypeInfo<JST>::Type &ResultType;

  ResultType operator()(v8::Handle<v8::Value> const &h) const
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
  typedef typename TypeInfo<JST>::Type const &ResultType;

  ResultType operator()(v8::Handle<v8::Value> const &h) const
    {
    typedef JSToNative<JST &> Cast;
    typedef typename Cast::ResultType NH;
    return Cast()(h);
    }
  };

template <typename JST> struct JSToNativeAbstract
  {
  typedef typename TypeInfo<JST>::NativeHandle ResultType;

  ResultType operator()( v8::Handle<v8::Value> const & ) const
    {
    return 0;
    }
  };

template <typename T> struct JSToNative<v8::Handle<T> >
  {
  typedef v8::Handle<T> ResultType;

  ResultType operator()( v8::Handle<T> const & h ) const
    {
    return h;
    }
  };

template <typename T> struct JSToNative<v8::Handle<T> const &> : JSToNative< v8::Handle<T> > {};
template <typename T> struct JSToNative<v8::Handle<T> &> : JSToNative< v8::Handle<T> > {};

template <typename T> struct JSToNative<v8::Local<T> >
  {
  typedef v8::Local<T> ResultType;

  ResultType operator()( v8::Local<T> const & h ) const
    {
    return h;
    }
  };

template <typename T> struct JSToNative<v8::Local<T> const &> : JSToNative< v8::Local<T> > {};
template <typename T> struct JSToNative<v8::Local<T> &> : JSToNative< v8::Local<T> > {};

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
  typedef typename TypeInfo<T>::NativeHandle ResultType;

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
  typedef typename TypeInfo<T>::NativeHandle ResultType;

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
template <typename V8Type, typename Ret, bool (v8::Value::*IsA)() const> struct JSToNativeV8PODType
  {
  typedef Ret ResultType;

  ResultType operator()(v8::Handle<v8::Value> const &h) const
    {
    return (h.IsEmpty() || !((*h)->*IsA)())
        ? 0
        : V8Type::Cast(*h)->Value();
    }
  };

template <typename ExtType> struct JSToNativeV8ExternalType
    : JSToNativeV8PODType<v8::External, ExtType, &v8::Value::IsExternal>
  {
  };

template <typename IntType> struct JSToNativeV8IntegerType
    : JSToNativeV8PODType<v8::Integer, IntType, &v8::Value::IsNumber>
  {
  };

template <typename NumType> struct JSToNativeV8NumberType
    : JSToNativeV8PODType<v8::Number, NumType, &v8::Value::IsNumber>
  {
  };
}

template <> struct JSToNative<void *> : JSToNativeV8ExternalType<void *> {};
template <> struct JSToNative<void const *> : JSToNativeV8ExternalType<void const *> {};

template <> struct JSToNative<int8_t> : JSToNativeV8IntegerType<int8_t> {};
template <> struct JSToNative<uint8_t> : JSToNativeV8IntegerType<uint8_t> {};
template <> struct JSToNative<int16_t> : JSToNativeV8IntegerType<int16_t> {};
template <> struct JSToNative<uint16_t> : JSToNativeV8IntegerType<uint16_t> {};
template <> struct JSToNative<int32_t> : JSToNativeV8IntegerType<int32_t> {};
template <> struct JSToNative<uint32_t> : JSToNativeV8IntegerType<uint32_t> {};
template <> struct JSToNative<int64_t> : JSToNativeV8IntegerType<int64_t> {};
template <> struct JSToNative<uint64_t> : JSToNativeV8IntegerType<uint64_t> {};

template <> struct JSToNative<double> : JSToNativeV8IntegerType<double> {};
template <> struct JSToNative<float> : JSToNativeV8IntegerType<float> {};

template <> struct JSToNative<bool>
  {
  typedef bool ResultType;

  ResultType operator()(v8::Handle<v8::Value> const &h) const
    {
    return h->BooleanValue();
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

template <typename NT> typename JSToNative<NT>::ResultType CastFromScript(v8::Handle<v8::Value> const &h)
  {
  typedef JSToNative<NT> F;
  return F()( h );
  }

}
#endif // XCONVERTFROMSCRIPT_H
