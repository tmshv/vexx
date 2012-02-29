#ifndef XCONVERTTOSCRIPT_H
#define XCONVERTTOSCRIPT_H

#include "XAssert"
#include "XScriptTypeInfo.h"
#include "XSignatureHelpers.h"
#include "XSignatureSpecialisations.h"

namespace cvv8
{

template <typename NT> struct NativeToJS
  {
  template <typename X>
  v8::Handle<v8::Value> operator()( X const & ) const;

private:
  typedef xCompileTimeAssertDef<false> NativeToJSMustBeSpecialized;
  };

template <typename NT> struct NativeToJS<NT *> : NativeToJS<NT> {};
template <typename NT> struct NativeToJS<NT const *> : NativeToJS<NT *> {};
template <typename NT> struct NativeToJS<const NT &> : NativeToJS<NT> {};
template <typename NT> struct NativeToJS<const NT> : NativeToJS<NT> {};

template <typename NT> struct NativeToJS<NT &>
  {
  typedef typename TypeInfo<NT>::Type & ArgType;
  v8::Handle<v8::Value> operator()( ArgType n ) const
    {
    typedef NativeToJS< typename TypeInfo<NT>::NativeHandle > Cast;
    return Cast()( &n );
    }
  };


namespace
{
template <typename IntegerT> struct NativeToJS_int_small
  {
  v8::Handle<v8::Value> operator()( IntegerT v ) const
    {
    return v8::Integer::New( static_cast<int32_t>(v) );
    }
  };

template <typename IntegerT> struct NativeToJSUIntSmall
  {
  v8::Handle<v8::Value> operator()( IntegerT v ) const
    {
    return v8::Integer::NewFromUnsigned( static_cast<uint32_t>(v) );
    }
  };

template <typename IntegerT> struct NativeToJSNumber
  {
  v8::Handle<v8::Value> operator()( IntegerT v ) const
    {
    return v8::Number::New( static_cast<double>(v) );
    }
  };

template <typename T> struct UselessConversionType
  {
  };
}

template <> struct NativeToJS<unsigned char> : NativeToJSUIntSmall<uint8_t> {};
template <> struct NativeToJS<int16_t> : NativeToJS_int_small<int16_t> {};
template <> struct NativeToJS<uint16_t> : NativeToJSUIntSmall<uint16_t> {};
template <> struct NativeToJS<int32_t> : NativeToJS_int_small<int32_t> {};
template <> struct NativeToJS<uint32_t> : NativeToJSUIntSmall<uint32_t> {};

template <> struct NativeToJS<int64_t> : NativeToJSNumber<int64_t> {};
template <> struct NativeToJS<uint64_t> : NativeToJSNumber<uint64_t> {};
template <> struct NativeToJS<double> : NativeToJSNumber<double> {};
template <> struct NativeToJS<float> : NativeToJSNumber<float> {};

// workarounds where long int != uint64
template <> struct NativeToJS< XIfElse< XSameType<unsigned long int,uint64_t>::Value,
    UselessConversionType<unsigned long>,
    unsigned long >::Type >
    : NativeToJSNumber<unsigned long int>
  {
  };

template <> struct NativeToJS< XIfElse< XSameType<long,int64_t>::Value,
    UselessConversionType<long>,
    long >::Type >
    : NativeToJSNumber<int64_t>
  {
  };

#ifdef X_HAS_LONG_LONG
template <> struct JSToNative< XIfElse<
    XSameType<long long int,int64_t>::Value,
    Detail::UselessConversionType<long long>,
    long long int >::Type > : JSToNative<int64_t>
  {
  };
#endif

template <> struct NativeToJS<bool>
  {
  v8::Handle<v8::Value> operator()( bool v ) const
    {
    return v8::Boolean::New( v );
    }
  };

template <typename T> struct NativeToJS< ::v8::Handle<T> >
  {
  typedef ::v8::Handle<T> HandleType;
  v8::Handle<v8::Value> operator()(HandleType const &li) const
    {
    return li;
    }
  };

template <typename T> struct NativeToJS< ::v8::Local<T> >
  {
  typedef ::v8::Local<T> HandleType;
  v8::Handle<v8::Value> operator()(HandleType const &li) const
    {
    return li;
    }
  };

template <typename T> struct NativeToJS< ::v8::Persistent<T> >
  {
  typedef ::v8::Persistent<T> HandleType;
  v8::Handle<v8::Value> operator()(HandleType const &li) const
    {
    return li;
    }
  };

template <> struct NativeToJS< ::v8::InvocationCallback >
  {
  v8::Handle<v8::Value> operator()(::v8::InvocationCallback const f) const
    {
    return ::v8::FunctionTemplate::New(f)->GetFunction();
    }
  };

template <> struct NativeToJS<char const *>
  {
  v8::Handle<v8::Value> operator()(char const *v) const
    {
    if( ! v ) return v8::Null();
    else return v8::String::New( v );
    }
  };

template <typename T> inline v8::Handle<v8::Value> CastToJS(T const &v)
  {
  typedef NativeToJS<T const> F;
  return F()( v );
  }

// Overloads to avoid ambiguity in certain calls.
static inline v8::Handle<v8::Value> CastToJS(char const *v)
  {
  typedef NativeToJS<char const *> F;
  return F()( v );
  }

static inline v8::Handle<v8::Value> CastToJS(v8::InvocationCallback v)
  {
  typedef NativeToJS<v8::InvocationCallback> F;
  return F()( v );
  }

static inline v8::Handle<v8::Value> CastToJS(char *v)
  {
  typedef NativeToJS<char const *> F;
  return F()( v );
  }

// Converts a native std::exception to a JS exception and throws
// that exception via v8::ThrowException().
template <> struct NativeToJS<std::exception>
  {
  v8::Handle<v8::Value> operator()( std::exception const & ex ) const
    {
    char const *msg = ex.what();
    return v8::Exception::Error(v8::String::New( msg ? msg : "unspecified std::exception" ));
    }
  };

template <> struct NativeToJS<std::runtime_error> : NativeToJS<std::exception> {};
template <> struct NativeToJS<std::range_error> : NativeToJS<std::exception> {};
template <> struct NativeToJS<std::logic_error> : NativeToJS<std::exception> {};

}
#endif // XCONVERTTOSCRIPT_H
