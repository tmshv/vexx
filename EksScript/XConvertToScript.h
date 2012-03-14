#ifndef XCONVERTTOSCRIPT_H
#define XCONVERTTOSCRIPT_H

#include "XAssert"
#include "XConvert.h"
#include "XScriptTypeInfo.h"
#include "XSignatureHelpers.h"
#include "XSignatureSpecialisations.h"

namespace XScriptConvert
{

namespace internal
{

template <typename NT> struct NativeToJS<NT *> : NativeToJS<NT> {};
template <typename NT> struct NativeToJS<NT const *> : NativeToJS<NT *> {};
template <typename NT> struct NativeToJS<const NT &> : NativeToJS<NT> {};
template <typename NT> struct NativeToJS<const NT> : NativeToJS<NT> {};

template <typename NT> struct NativeToJS<NT &>
  {
  typedef typename XScriptTypeInfo<NT>::Type & ArgType;
  XScriptObject operator()( ArgType n ) const
    {
    typedef NativeToJS< typename XScriptTypeInfo<NT>::NativeHandle > Cast;
    return Cast()( &n );
    }
  };


namespace
{
template <typename IntegerT> struct NativeToJS_int_small
  {
  XScriptObject operator()( IntegerT v ) const
    {
    return XScriptObject(v);
    }
  };

template <typename IntegerT> struct NativeToJSUIntSmall
  {
  XScriptObject operator()( IntegerT v ) const
    {
    return XScriptObject(v);
    }
  };

template <typename IntegerT> struct NativeToJSNumber
  {
  XScriptObject operator()( IntegerT v ) const
    {
    return XScriptObject(v);
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
  XScriptObject operator()( bool v ) const
    {
    return XScriptObject( v );
    }
  };

//template <typename T> struct NativeToJS< ::v8::Handle<T> >
//  {
//  typedef ::v8::Handle<T> HandleType;
//  XScriptObject operator()(HandleType const &li) const
//    {
//    return li;
//    }
//  };
/*
template <typename T> struct NativeToJS< ::v8::Local<T> >
  {
  typedef ::v8::Local<T> HandleType;
  XScriptObject operator()(HandleType const &li) const
    {
    return li;
    }
  };

template <typename T> struct NativeToJS< ::v8::Persistent<T> >
  {
  typedef ::v8::Persistent<T> HandleType;
  XScriptObject operator()(HandleType const &li) const
    {
    return li;
    }
  };

template <> struct NativeToJS< ::v8::InvocationCallback >
  {
  XScriptObject operator()(::v8::InvocationCallback const ) const
    {
    xAssertFail();
    return XScriptObject();
    //return ::v8::FunctionTemplate::New(f)->GetFunction();
    }
  };*/

template <> struct NativeToJS<char const *>
  {
  XScriptObject operator()(char const *v) const
    {
    if(!v) return XScriptObject();
    else return XScriptObject( v );
    }
  };

template <> struct NativeToJS<QString>
  {
  XScriptObject operator()(QString v) const
    {
    return XScriptObject(v);
    }
  };

// Converts a native std::exception to a JS exception and throws
// that exception via v8::ThrowException().
template <> struct NativeToJS<std::exception>
  {
  XScriptObject operator()( std::exception const &) const
    {
    xAssertFail();
    return XScriptObject();
    //char const *msg = ex.what();
    //return v8::Exception::Error(v8::String::New( msg ? msg : "unspecified std::exception" ));
    }
  };

template <> struct NativeToJS<std::runtime_error> : NativeToJS<std::exception> {};
template <> struct NativeToJS<std::range_error> : NativeToJS<std::exception> {};
template <> struct NativeToJS<std::logic_error> : NativeToJS<std::exception> {};

}

// Overloads to avoid ambiguity in certain calls.
static inline XScriptObject to(char const *v)
  {
  typedef internal::NativeToJS<char const *> F;
  return F()( v );
  }

//static inline XScriptObject to(v8::InvocationCallback v)
//  {
//  typedef internal::NativeToJS<v8::InvocationCallback> F;
//  return F()( v );
//  }

static inline XScriptObject to(char *v)
  {
  typedef internal::NativeToJS<char const *> F;
  return F()( v );
  }

template <typename T> inline XScriptObject to(T const &v)
  {
  typedef internal::NativeToJS<T const> F;
  return F()( v );
  }

}

#endif // XCONVERTTOSCRIPT_H
