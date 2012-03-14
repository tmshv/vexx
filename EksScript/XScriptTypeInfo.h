#ifndef XSCRIPTTYPEINFO_H
#define XSCRIPTTYPEINFO_H

template <typename T, typename NHT = T *> struct XScriptTypeInfoCore
  {
  typedef T Type;
  typedef NHT NativeHandle;
  static char const *TypeName;
  };

template <typename T> struct XScriptTypeInfoBase : XScriptTypeInfoCore<T, T*> {};

template <typename T> struct XScriptTypeInfoBase<T const> : XScriptTypeInfoBase<T> {};
template <typename T> struct XScriptTypeInfoBase<T *> : XScriptTypeInfoBase<T> {};
template <typename T> struct XScriptTypeInfoBase<T const *> : XScriptTypeInfoBase<T> {};
template <typename T> struct XScriptTypeInfoBase<T const &> : XScriptTypeInfoBase<T> {};
template <typename T> struct XScriptTypeInfoBase<T &> : XScriptTypeInfoBase<T> {};

//template <typename T> struct XScriptTypeInfoBase< v8::Handle<T> >
//{
//    typedef v8::Handle<T> Type;
//    typedef v8::Handle<T> NativeHandle;
//};

template <typename T> class XScriptTypeInfo : public XScriptTypeInfoBase<T> {};

#define CVV8_TypeInfo_DECL(X) template <> struct XScriptTypeInfo<X> : public XScriptTypeInfoBase<X> \
{ static const char *TypeName; }

#define CVV8_TypeInfo_IMPL(X,NAME) char const * XScriptTypeInfo<X>::TypeName = NAME


#endif // XSCRIPTTYPEINFO_H
