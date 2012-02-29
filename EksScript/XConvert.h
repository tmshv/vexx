#ifndef XCONVERT_H
#define XCONVERT_H

#include "XScriptTypeInfo.h"

namespace cvv8
{

template <typename JST> struct JSToNative
  {
  typedef typename TypeInfo<JST>::NativeHandle ResultType;

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

#endif // XCONVERT_H
