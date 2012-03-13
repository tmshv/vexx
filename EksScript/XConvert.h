#ifndef XCONVERT_H
#define XCONVERT_H

#include "XScriptTypeInfo.h"

namespace XScriptConvert
{


template <typename T> inline const T &ptrMatcher(T *in, bool& valid)
{
  if(!in)
  {
    static T o;
    valid = false;
    return o;
  }
  return *in;
}

template <typename Out, typename In> Out match(In in, bool &valid)
{
  valid = true;
  return (Out)in;
}

namespace internal
{

template <typename JST> struct JSToNative
  {
  typedef typename XScriptTypeInfo<JST>::NativeHandle ResultType;

  //! Must be specialized to be useful.
  ResultType operator()(v8::Handle<v8::Value> const &h) const;
  };

template <typename NT> struct NativeToJS
  {
  template <typename X>
  v8::Handle<v8::Value> operator()( X const & ) const;

private:
  typedef xCompileTimeAssertDef<false> NativeToJSMustBeSpecialized;
  };

}
}

#endif // XCONVERT_H
