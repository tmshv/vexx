#ifndef XINTERFACE_H
#define XINTERFACE_H

namespace cvv8
{
  template <typename T> struct NativeToJSCopyableType
  {
    v8::Handle<v8::Value> operator()( T const * n ) const
    {
      T *out = 0;
      v8::Handle<v8::Object> obj = ClassCreator<T>::Instance().NewInstance( 0, 0, out );
      *out = *n;
      return obj;
    }
    v8::Handle<v8::Value> operator()( T const & n ) const
    {
      return this->operator()( &n );
    }
  };

  template <typename T, typename CTORS> class ClassCreatorCopyableFactory
  {
  public:
    typedef T* ReturnType;

    static T *Create(v8::Persistent<v8::Object> & jsSelf, v8::Arguments const & argv)
    {
      typedef cvv8::CtorArityDispatcher<CTORS> Proxy;
      T *b = Proxy::Call(argv);
      if(b)
      {
        NativeToJSMap<T>::Insert(jsSelf, b);
      }
      v8::V8::AdjustAmountOfExternalAllocatedMemory((int)sizeof(*b));
      return b;
    }

    static void Delete(T *obj)
    {
      NativeToJSMap<T>::Remove(obj);
      delete obj;
      v8::V8::AdjustAmountOfExternalAllocatedMemory(-((int)sizeof(*obj)));
    }
  };

  template <typename T> inline const T &PtrMatcher(T *in, bool& valid)
  {
    if(!in)
    {
      static T o;
      valid = false;
      return o;
    }
    return *in;
  }

#define X_SCRIPTABLE_CONSTRUCTOR_DEF(variable, type, n) variable,

#define X_SCRIPTABLE_BUILD_CONSTRUCTABLE_TYPEDEF(name, type, ...)  typedef cvv8::Signature< type (X_EXPAND_ARGS(X_SCRIPTABLE_CONSTRUCTOR_DEF, type, __VA_ARGS__) cvv8::CtorForwarder<type *(const type &)>, cvv8::CtorForwarder<type *()> )> name;

#define X_SCRIPTABLE_BUILD_CONSTRUCTABLE(type, ...) \
X_SCRIPTABLE_BUILD_CONSTRUCTABLE_TYPEDEF(type##Ctors, type, __VA_ARGS__) \
template <> class ClassCreator_Factory<type> : public ClassCreatorCopyableFactory<type, type##Ctors> {};

#define X_SCRIPTABLE_MATCHERS(type) \
template <> const type& Match<const type&, type*>(type *in, bool& valid) { return PtrMatcher<type>(in, valid); }

#define X_SCRIPTABLE_TYPE_BASE(type)  \
  namespace cvv8 { \
  CVV8_TypeName_DECL((type)); \
  template <> struct JSToNative<type> : JSToNative_ClassCreator<type> {}; \
  X_SCRIPTABLE_MATCHERS(type) \
  }

#define X_SCRIPTABLE_TYPE_COPYABLE(type, ...) X_SCRIPTABLE_TYPE_BASE(type) \
  namespace cvv8 { \
  template <> struct NativeToJS<type> : public NativeToJSCopyableType<type> {}; \
  X_SCRIPTABLE_BUILD_CONSTRUCTABLE(type, __VA_ARGS__) \
  }

#define X_SCRIPTABLE_TYPE_NOT_COPYABLE(type) X_SCRIPTABLE_TYPE_BASE(type)

#define X_IMPLEMENT_SCRIPTABLE_TYPE_BASE(type, name) \
  namespace cvv8 { \
  CVV8_TypeName_IMPL((type), name); \
  }

#define X_IMPLEMENT_SCRIPTABLE_TYPE_COPYABLE(type, name) X_IMPLEMENT_SCRIPTABLE_TYPE_BASE(type, name)

#define X_IMPLEMENT_SCRIPTABLE_TYPE_NOT_COPYABLE(type, name) X_IMPLEMENT_SCRIPTABLE_TYPE_BASE(type, name)
}

template <typename T> class XInterface
  {
public:
  XInterface()
    {
    }

  template <typename GETTYPE,
            typename SETTYPE,
            typename cvv8::ConstMethodSignature<T, GETTYPE ()>::FunctionType GETTERMETHOD,
            typename cvv8::MethodSignature<T, void (SETTYPE)>::FunctionType SETTERMETHOD>
  void addProperty(const char *name)
    {
    typedef cvv8::ClassCreator<T> CC;
    CC& cc(CC::Instance());
    v8::Handle<v8::ObjectTemplate> const &proto(cc.Prototype());

    v8::AccessorGetter getter = cvv8::ConstMethodToGetter<T, GETTYPE (), GETTERMETHOD>::Get;
    v8::AccessorSetter setter = cvv8::MethodToSetter<T, SETTYPE, SETTERMETHOD>::Set;

    proto->SetAccessor(v8::String::New(name), getter, setter);
    }
  };


#endif // XINTERFACE_H
