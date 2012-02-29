#ifndef XCONVERTSCRIPTSTL_H
#define XCONVERTSCRIPTSTL_H

#include "XConvertFromScript.h"
#include "XConvertToScript.h"

namespace cvv8
{

template <>struct JSToNative<std::string>
  {
  typedef std::string ResultType;

  ResultType operator()( v8::Handle<v8::Value> const & h ) const
    {
    static const std::string emptyString;
    v8::String::Utf8Value const utf8String( h );
    const char* s = *utf8String;
    return s
      ? std::string(s, utf8String.length())
      : emptyString;
    }
  };

template <> struct JSToNative<std::string const &> : public JSToNative<std::string> {};



template <> struct NativeToJS<std::string>
  {
  v8::Handle<v8::Value> operator()(std::string const &v) const
    {
    char const *const cstr  = v.data();
    return cstr ? v8::String::New( cstr, static_cast<int>( v.size() ) ) : v8::String::New("",0);
    }
  };

}

#endif // XCONVERTSCRIPTSTL_H
