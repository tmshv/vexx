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

namespace cvv8
{

template <typename ListT> struct NativeToJSList
  {
  v8::Handle<v8::Value> operator()( ListT const & li ) const
    {
    typedef typename ListT::const_iterator IT;
    IT it = li.begin();
    const size_t sz = li.size();
    v8::Handle<v8::Array> rv( v8::Array::New( static_cast<int>(sz) ) );
    for( int i = 0; li.end() != it; ++it, ++i )
      {
      rv->Set( v8::Integer::New(i), CastToJS( *it ) );
      }
    return rv;
    }
  };

template <typename T> struct NativeToJS<std::list<T> > : NativeToJSList< std::list<T> > {};
template <typename T> struct NativeToJS<std::vector<T> > : NativeToJSList< std::vector<T> > {};

template <typename MapT> struct NativeToJSMap
  {
  v8::Handle<v8::Value> operator()(MapT const &li) const
    {
    typedef typename MapT::const_iterator IT;
    IT it( li.begin() );
    v8::Handle<v8::Object> rv( v8::Object::New() );
    for( int i = 0; li.end() != it; ++it, ++i )
      {
      rv->Set( CastToJS( (*it).first ), CastToJS( (*it).second ) );
      }
    return rv;
    }
  };

template <typename KeyT, typename ValT> struct NativeToJS<std::map<KeyT, ValT> > : NativeToJSMap<std::map<KeyT,ValT> > {};

template <typename ListT, typename ValueType = typename ListT::value_type> struct JSToNativeList
  {
  typedef ListT ResultType;

  ResultType operator()( v8::Handle<v8::Value> jv ) const
    {
    typedef ValueType VALT;
    ListT li;
    if( jv.IsEmpty() || ! jv->IsArray() ) return li;
    v8::Handle<v8::Array> ar( v8::Array::Cast(*jv) );
    uint32_t ndx = 0;
    for( ; ar->Has(ndx); ++ndx )
      {
      li.push_back( CastFromJS<VALT>( ar->Get(v8::Integer::New(ndx)) ) );
      }
    return li;
    }
  };

template <typename T> struct JSToNative<std::list<T> > : JSToNativeList<std::list<T> > {};
template <typename T> struct JSToNative<std::vector<T> > : JSToNativeList<std::vector<T> > {};

#if 0 // untested code
/**
       UNTESTED!

       Intended as a base class for JSToNative specializations
       which proxy a std::map-like map.
    */
template <typename MapT,
          typename KeyType = typename MapT::mapped_type,
          typename ValueType = typename MapT::value_type>
struct JSToNative_map
  {
  typedef MapT ResultType;
  /**
           Converts jv to a MapT object.

           If jv->IsObject() then the returned object is populated from
           jv, otherwise the returned object is empty. Since it is
           legal for an object to be empty, it is not generically
           possible to know if this routine got an empty Object
           or a non-Object handle.
        */
  ResultType operator()( v8::Handle<v8::Value> jv ) const
    {
    typedef ValueType VALT;
    MapT map;
    if( jv.IsEmpty() || ! jv->IsObject() ) return map;
    Local<Object> obj( Object::Cast(*jv) );
    Local<Array> ar( obj->GetPropertyNames() );
    uint32_t ndx = 0;
    for( ; ar->Has(ndx); ++ndx )
      {
      Local<Value> const & k = ar->Get(Integer::New(ndx));
      if( ! obj->HasRealNamedProperty(k) ) continue;
      map[CastFromJS<KeyType>(k)] = CastFromJS<ValueType>(obj->Get(k));
      }
    return map;
    }
  };
#endif
/**
       A utility class for building up message strings, most notably
       exception messages, using a mixture of native and JS message
       data.

       It is used like a std::ostream:

       @code
       StringBuffer msg;
       msg << "Could not set property "
           << "'" << propName
           <<"' on object " << myJSObject << '!';
       return v8::ThrowException(msg.toError());
       // or:
       return Toss(msg);
       @endcode
    */
class StringBuffer
  {
private:
  std::ostringstream os;
public:
  /**
           Initializes the message stream.
        */
  StringBuffer() : os()
    {
    }
  StringBuffer(StringBuffer const & other) : os(other.os.str())
    {
    }
  StringBuffer & operator=(StringBuffer const & other)
    {
    this->os.str(other.os.str());
    return *this;
    }

  /**
           Empties out the message buffer. This invalidates any value
           returned from previous calls to the (char const *)
           operator.
        */
  inline void Clear()
    {
    this->os.str("");
    }

  /**
           Returns a copy of the current message content.
        */
  inline std::string Content() const
    {
    return this->os.str();
    }

  /**
           Converts the message state to a JS string.
        */
  inline operator v8::Handle<v8::Value>() const
    {
    return CastToJS<std::string>( this->os.str() );
    }

  /**
           Converts the message state to a v8::String (for use
           with v8::Exception::Error() and friends).
        */
  inline operator v8::Handle<v8::String>() const
    {
    std::string const & str(this->os.str());
    char const * cstr = str.c_str();
    return v8::String::New( cstr ? cstr : "", cstr ? str.size() : 0 );
    }

  /**
           Appends to the message using CastFromJS<std::string>(t)
        */
  template <typename T>
  inline StringBuffer & operator<<( v8::Handle<T> const & t )
    {
    this->os << CastFromJS<std::string>( t );
    return *this;
    }
  /**
           Appends to the message using CastFromJS<std::string>(t)

           Reminder to self: if this function is changed to take
           a const reference instead of a copy, we get overload
           ambiguity errors in some contexts. See:

           http://code.google.com/p/v8-juice/issues/detail?id=19

           (And thanks to Feng Fillano for reporting and localizing
           the problem.)
        */
  template <typename T>
  inline StringBuffer & operator<<( v8::Local<T> const t )
    {
    this->os << CastFromJS<std::string>( t );
    return *this;
    }

  /**
           Appends t to the message via std::ostream<<.
        */
  template <typename T>
  inline StringBuffer & operator<<( T const t)
    {
    this->os << t;
    return *this;
    }

  /**
           Returns this buffer's value wrapped in
           a JS Error object.
        */
  v8::Local<v8::Value> toError() const
    {
    return v8::Exception::Error(*this);
    }
  };

/** Outputs sb.Content() to os and returns os. */
inline std::ostream & operator<<( std::ostream & os, StringBuffer const & sb )
  {
  return os << sb.Content();
  }


/**
        Requi
        "Lexically casts" msg to a string and throws a new JS-side
        Error. ValT may be any type which can be sent to StringBuffer's
        ostream operator.
        
        The return value is the result of calling
        v8::ThrowException() (Undefined except in the face of a
        serious internal error like OOM, i'm told by the v8 devs).
    */
template <typename ValT>
inline v8::Handle<v8::Value> Toss( ValT const & msg )
  {
  return v8::ThrowException((StringBuffer() << msg).toError());
  }

/**
        Overload to avoid an ambiguity (and it's more efficient than
        the default impl).
    */
inline v8::Handle<v8::Value> Toss( char const * msg )
  {
  return v8::ThrowException(v8::Exception::Error(v8::String::New( msg ? msg : "Unspecified error.")));
  }

/**
        Eqivalent to v8::ThrowException(err). Note that if err is not an
        Error object, the JS side will not get an Error object. e.g. if err
        is-a String then the JS side will see the error as a string.

        The reason this does not convert err to an Error is because the v8
        API provides no way for us to know if err is already an Error object.
        This function is primarily intended to be passed the results of
        CastToJS(std::exception), which generates Error objects.
    */
inline v8::Handle<v8::Value> Toss( v8::Handle<v8::Value> const & err )
  {
  return v8::ThrowException(err);
  }

/**
        Efficiency overload.
    */
inline v8::Handle<v8::Value> Toss( StringBuffer const & msg )
  {
  return v8::ThrowException(msg.toError());
  }
/**
        Like Toss(Handle<Value>), but converts err to a string and creates an
        Error object, which is then thrown.
    */
inline v8::Handle<v8::Value> TossAsError( v8::Handle<v8::Value> const & err )
  {
  return Toss(v8::Exception::Error(err->ToString()));
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
  typedef typename JSToNative<T>::ResultType ResultType;
  /**
           Default impl simply returns CastFromJS<T>(v).
           Specializations are allowed to store the result of the
           conversion, as long as they release it when the destruct.
           See ArgCaster<char const *> for an example of that.
        */
  inline ResultType ToNative( v8::Handle<v8::Value> const & v ) const
    {
    return CastFromJS<T>( v );
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
    typedef JSToNative<std::string> C;
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
    typedef typename TypeInfo<ReturnType>::Type RType;
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
    typedef typename TypeInfo<ReturnType>::Type T;
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
  typedef typename TypeInfo<T>::NativeHandle ReturnType;
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
  typedef typename TypeInfo<T>::NativeHandle ReturnType;
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
  typedef typename TypeInfo<T>::NativeHandle ReturnType;
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
  typedef typename TypeInfo<T>::NativeHandle ReturnType;
  /** Writes an error message to errmsg and returns 0. */
  static ReturnType Call( v8::Arguments const &  argv )
    {
    StringBuffer msg;
    msg << "No native constructor was defined for "<<argv.Length()<<" arguments!\n";
    throw std::range_error(msg.Content().c_str());
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
  typedef typename TypeInfo<RT>::NativeHandle NativeHandle;
  static NativeHandle Call( v8::Arguments const & argv )
    {
    typedef typename XPlainType<RT>::Type Type;
    typedef Detail::CtorFwdDispatchList<Type, CtorList> Proxy;
    return Proxy::Call( argv );
    }
  };

} // namespaces


#endif /* CODE_GOOGLE_COM_P_V8_CONVERT_CORE_HPP_INCLUDED */
