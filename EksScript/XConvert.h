#ifndef XCONVERT_H
#define XCONVERT_H

#include "XScriptTypeInfo.h"
#include "XScriptValue.h"

namespace XScriptConvert
{

template <typename T, typename U> T *castFromBase(U *ptr)
  {
  return dynamic_cast<T*>(ptr);
  }

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
  ResultType operator()(XScriptValue const &h) const;
  };

template <typename NT> struct NativeToJS
  {
  template <typename X>
  XScriptValue operator()( X const & ) const;

private:
  typedef xCompileTimeAssertDef<false> NativeToJSMustBeSpecialized;
  };

}



/**
       ArgCaster is a thin wrapper around CastFromJS(), and primarily
       exists to give us a way to convert JS values to (char const *)
       for purposes of passing them to native functions. The main
       difference between this type and JSToNative<T> is that this
       interface explicitly allows for the conversion to be stored by
       an instance of this type. That allows us to get more lifetime
       out of converted values in certain cases (namely (char const*)).

       The default implementation is suitable for all cases which
       JSToNative<T> supports, but specializations can handle some of
       the corner cases which JSToNative cannot (e.g. (char const *)).

       Added 20091121.
    */
template <typename T>
struct ArgCaster
  {
  typedef typename XScriptConvert::internal::JSToNative<T>::ResultType ResultType;
  /**
           Default impl simply returns CastFromJS<T>(v).
           Specializations are allowed to store the result of the
           conversion, as long as they release it when the destruct.
           See ArgCaster<char const *> for an example of that.
        */
  inline ResultType ToNative( XScriptValue const & v ) const
    {
    return XScriptConvert::from<T>( v );
    }
  /**
            To eventually be used for some internal optimizations.
        */
  enum { HasConstOp = 1 };
  };
/**
       Specialization for (char const *). The value returned from
       ToNative() is guaranteed to be valid as long as the ArgCaster
       object is alive or until ToNative() is called again (which will
       almost certainly change the pointer). Holding a pointer to the
       ToNative() return value after the ArgCaster is destroyed will
       lead to undefined behaviour. Likewise, fetching a pointer, then
       calling ToNative() a second time, will invalidate the first
       pointer.

       BEWARE OF THESE LIMITATIONS:

       1) This will only work properly for nul-terminated strings,
       and not binary data!

       2) Do not use this to pass (char const *) as a function
       parameter if that function will hold a copy of the pointer
       after it returns (as opposed to copying/consuming the
       pointed-to-data before it returns) OR if it returns the
       pointer passed to it. Returning is a specific corner-case
       of "holding a copy" for which we cannot guaranty the lifetime
       at the function-binding level.

       3) Do not use the same ArgCaster object to convert multiple
       arguments, as each call to ToNative() will invalidate the
       pointer returned by previous calls.

       4) The to-string conversion uses whatever encoding
       JSToNative<std::string> uses.

       Violating any of those leads to undefined behaviour, and
       very possibly memory corruption for cases 2 or 3.
    */
template <>
struct ArgCaster<char const *>
  {
private:
  /**
            Reminder to self: we cannot use v8::String::Utf8Value
            here because at the point the bindings call ToNative(),
            v8 might have been unlocked, at which point dereferencing
            the Utf8Value becomes illegal.
        */
  std::string val;
  typedef char Type;
public:
  typedef Type const * ResultType;
  /**
           Returns the toString() value of v unless:

           - v.IsEmpty()
           - v->IsNull()
           - v->IsUndefined()

           In which cases it returns 0.

           The returned value is valid until:

           - ToNative() is called again.
           - This object is destructed.
        */
  ResultType ToNative( XScriptValue const & )
    {/*
    typedef XScriptConvert::internal::JSToNative<std::string> C;
    if( !v.isValid() )
      {
      return 0;
      }
    this->val = C()( v );*/
    xAssertFail();
    return "";//this->val.c_str();
    }
  };
}

#endif // XCONVERT_H
