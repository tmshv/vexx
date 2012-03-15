#ifndef XCONVERTSCRIPTSTL_H
#define XCONVERTSCRIPTSTL_H

#include "XConvertFromScript.h"
#include "XConvertToScript.h"

namespace XScriptConvert
{

namespace internal
{

template <>struct JSToNative<std::string>
  {
  typedef std::string ResultType;

  ResultType operator()( XScriptValue const & ) const
    {
    /*static const std::string emptyString;
    v8::String::Utf8Value const utf8String( h );
    const char* s = *utf8String;
    return s
      ? std::string(s, utf8String.length())
      : emptyString;*/
    }
  };

template <> struct JSToNative<std::string const &> : public JSToNative<std::string> {};



template <> struct NativeToJS<std::string>
  {
  XScriptValue operator()(std::string const &v) const
    {
    return XScriptValue(QString::fromStdString(v));
    }
  };


template <typename ListT> struct NativeToJSList
  {
  XScriptValue operator()( ListT const & li ) const
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

template <typename MapT> struct NativeToJSLookup
  {
  XScriptValue operator()(MapT const &li) const
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

template <typename KeyT, typename ValT> struct NativeToJS<std::map<KeyT, ValT> > : NativeToJSLookup<std::map<KeyT,ValT> > {};

template <typename ListT, typename ValueType = typename ListT::value_type> struct JSToNativeList
  {
  typedef ListT ResultType;

  ResultType operator()( XScriptValue jv ) const
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

}
}

#endif // XCONVERTSCRIPTSTL_H
