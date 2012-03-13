#if !defined(CODE_GOOGLE_COM_P_V8_CONVERT_CORE_HPP_INCLUDED)
#define CODE_GOOGLE_COM_P_V8_CONVERT_CORE_HPP_INCLUDED 1

#include <v8.h>
#include <string>
#include <cstring>
#include <list>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>
#include "XSignature.h"
#include "XSignatureHelpers.h"
#include "XSignatureSpecialisations.h"
#include "XTemplateMetaHelpers.h"
#include "XAssert"
#include "XScriptTypeInfo.h"
#include "XConvertScriptSTL.h"

namespace cvv8
{
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
  inline ResultType ToNative( v8::Handle<v8::Value> const & v ) const
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
  ResultType ToNative( v8::Handle<v8::Value> const & v )
    {
    typedef XScriptConvert::internal::JSToNative<std::string> C;
    if( v.IsEmpty() || v->IsNull() || v->IsUndefined() )
      {
      return 0;
      }
    this->val = C()( v );
    return this->val.c_str();
    }
  /**
            To eventually be used for some internal optimizations.
        */
  enum { HasConstOp = 0 };
  };

#if !defined(DOXYGEN)
namespace Detail {
/**
            Default (unimplemented) CtorForwarderProxy impl. A helper
            for the CtorForwarder class. All specializations except
            the 0-arity one are generated from script code.
        */
template <typename Sig, int Arity = sl::Arity< XSignature<Sig> >::Value >
struct CtorForwarderProxy
  {
  typedef typename XSignature<Sig>::ReturnType ReturnType;
  static ReturnType Call( v8::Arguments const & );
  };

//! Specialization for 0-arity ctors.
template <typename Sig>
struct CtorForwarderProxy<Sig,0>
  {
  typedef typename XSignature<Sig>::ReturnType ReturnType;
  static ReturnType Call( v8::Arguments const & )
    {
    typedef typename XScriptTypeInfo<ReturnType>::Type RType;
    return new RType;
    }
  };
//! Specialization for ctors taking (v8::Arguments const &).
template <typename Sig>
struct CtorForwarderProxy<Sig,-1>
  {
  typedef typename XSignature<Sig>::ReturnType ReturnType;
  static ReturnType Call( v8::Arguments const & argv )
    {
    typedef typename XScriptTypeInfo<ReturnType>::Type T;
    return new T(argv);
    }
  };

}
#endif
/**
       A utility type to help forward v8::Arguments to native
       constructors. This type is intended to assist in the creation
       of ctor functions for JS-bound C++ classes.

       Requirements:

       - Sig is "almost" a function-signature-style type, but
       because ctors don't have a return value in the conventional
       sense, we have to use a tiny workaround: Sig should be passed
       in like this from client code:

       @code
       typedef CtorForwarder<T * (argType1, argType2)> CF;
       @endcode

       - (new T(...)) must be legal, taking a number of
       arguments equal to Sig's Arity.

       - All arguments to the native ctor must be convertible
       using CastFromJS().

        Example:

       @code
       typedef CtorForwarder<MyType *()> CF0;
       typedef CtorForwarder<MyType *(int)> CF1;
       typedef CtorForwarder<MyType *(double,int)> CF2;
       typedef CtorForwarder<MyType *(v8::Arguments const &)> CFAny;
       @endcode

       @see CtorArityDispatcher
    */
template <typename Sig>
struct CtorForwarder : XSignature<Sig>
  {
  typedef XSignature<Sig> STL;
  //typedef typename tmp::AddPointer<typename STL::ReturnType>::Type ReturnType;
  typedef typename STL::ReturnType ReturnType;
  /**
            If (argv.Length()>=Arity) or Arity is less than 0,
            then the constructor is called with Arity arguments
            (if it >=0) or with 1 v8::Arguments parameter (for Arity<0).

            Returns the result of (new Type(...)), transfering ownership
            to the caller.

            May propagate native exceptions.
        */
  static ReturnType Call( v8::Arguments const & argv )
    {
    enum { Arity = sl::Arity< STL >::Value };
    typedef Detail::CtorForwarderProxy<Sig> Proxy;
    return Proxy::Call( argv );
    }
  };

#if !defined(DOXYGEN)
namespace Detail
{

/**
           Internal dispatch routine. CTOR _must_ be a CtorForwarder implementation
           (or interface-compatible).
        */
template <typename T,typename CTOR>
struct CtorFwdDispatch
  {
  typedef typename XScriptTypeInfo<T>::NativeHandle ReturnType;
  static ReturnType Call( v8::Arguments const &  argv )
    {
    return CTOR::Call( argv );
    }
  };
/**
           Internal dispatch end-of-list routine.
        */
template <typename T>
struct CtorFwdDispatch<T, XNilType>
  {
  typedef typename XScriptTypeInfo<T>::NativeHandle ReturnType;
  static ReturnType Call( v8::Arguments const & )
    {
    return 0;
    }
  };
/**
           Internal type to dispatch a v8::Arguments list to one of
           several a bound native constructors, depending on on the
           argument count.

           List MUST be a Signature< ... > containing ONLY
           CtorFowarder types (or compatible).
        */
template <typename T,typename List>
struct CtorFwdDispatchList
  {
  typedef typename XScriptTypeInfo<T>::NativeHandle ReturnType;
  /**
               Tries to dispatch Arguments to one of the constructors
               in the List type, based on the argument count.
             */
  static ReturnType Call( v8::Arguments const &  argv )
    {
    typedef typename List::Head CTOR;
    typedef typename List::Tail Tail;
    enum { Arity = (0==sl::Index<v8::Arguments const &,CTOR>::Value)
           ? -1 : sl::Length<CTOR>::Value
           };
    return ( (Arity < 0) || (Arity == argv.Length()) )
        ? CtorFwdDispatch< T, CTOR >::Call(argv )
        : CtorFwdDispatchList<T,Tail>::Call(argv);
    }
  };
/**
           End-of-list specialization.
        */
template <typename T>
struct CtorFwdDispatchList<T,XNilType>
  {
  typedef typename XScriptTypeInfo<T>::NativeHandle ReturnType;
  /** Writes an error message to errmsg and returns 0. */
  static ReturnType Call( v8::Arguments const &  argv )
    {
    QString error = QString("No native constructor was defined for %1 arguments!\n").arg(argv.Length());
    throw std::range_error(error.toUtf8().data());
    return 0;
    }
  };
}
#endif // !DOXYGEN

/**
        Proxies a list of constructors from v8::Arguments.

        CtorList must be-a a Signature type in this form:

        @code
        typedef Signature<MyType ( // may optionally be (MyType *) - same effect
            CtorForwarder<MyType *()>,
            CtorForwarder<MyType *(char const *)>,
            CtorForwarder<MyType *( int, double )>,
            CtorForwarder<MyType *( v8::Arguments const &)>
        )> Ctors;
        @endcode

        All entries in the typelist must be interface-compatible with
        CtorForwarder. No two entries should have the same number
        of arguments with one exception: an InvocationCallback-like
        function taking (v8::Arguments const &) can be used as a
        catch-all for any number of arguments. If used, it must be
        the LAST entry because it will always match any argument
        count (and will therefore trump any which (would) come after
        it.

        The ctors are dispatched based solely on the argument count,
        not their types. The first one with a matching arity
        is called.

        IN THEORY (untested), the factories passed in the list may
        legally return a type publically derived from
        CtorList::ReturnType.
    */
template <typename CtorList>
struct CtorArityDispatcher
  {
  typedef typename CtorList::ReturnType RT;
  typedef typename XScriptTypeInfo<RT>::NativeHandle NativeHandle;
  static NativeHandle Call( v8::Arguments const & argv )
    {
    typedef typename XPlainType<RT>::Type Type;
    typedef Detail::CtorFwdDispatchList<Type, CtorList> Proxy;
    return Proxy::Call( argv );
    }
  };

} // namespaces


#endif /* CODE_GOOGLE_COM_P_V8_CONVERT_CORE_HPP_INCLUDED */
