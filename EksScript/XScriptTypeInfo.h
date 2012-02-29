#ifndef XSCRIPTTYPEINFO_H
#define XSCRIPTTYPEINFO_H

namespace cvv8
{

template <typename T, typename NHT = T *> struct TypeInfoCore
  {
  typedef T Type;
  typedef NHT NativeHandle;
  static char const *TypeName;
  };

template <typename T> struct TypeInfoBase : TypeInfoCore<T, T*> {};

template <typename T> struct TypeInfoBase<T const> : TypeInfoBase<T> {};
template <typename T> struct TypeInfoBase<T *> : TypeInfoBase<T> {};
template <typename T> struct TypeInfoBase<T const *> : TypeInfoBase<T> {};
template <typename T> struct TypeInfoBase<T const &> : TypeInfoBase<T> {};
template <typename T> struct TypeInfoBase<T &> : TypeInfoBase<T> {};

template <typename T> struct TypeInfoBase< v8::Handle<T> >
{
    typedef v8::Handle<T> Type;
    typedef v8::Handle<T> NativeHandle;
};

template <typename T> class TypeInfo : public TypeInfoBase<T> {};

#define CVV8_TypeInfo_DECL(X) template <> struct TypeInfo<X> : public TypeInfoBase<X> \
{ static const char *TypeName; }

#define CVV8_TypeInfo_IMPL(X,NAME) char const * TypeInfo<X>::TypeName = NAME

}

#endif // XSCRIPTTYPEINFO_H
