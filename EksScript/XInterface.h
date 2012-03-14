#ifndef XINTERFACE_H
#define XINTERFACE_H

#include "v8.h"
#include "XScriptGlobal.h"
#include "XMetaType"
#include "XProperty"
#include "XVariant"
#include "XHash"
#include "XMacroHelpers"
#include "XScriptFunction.h"
#include "cvv8/properties.hpp"
#include "cvv8/ClassCreator.hpp"
#include "XConvertToScript.h"

class EKSSCRIPT_EXPORT XInterfaceBase
  {
XProperties:
  XROSpecificProperty(v8::Handle<v8::FunctionTemplate>, constructor, public, protected);
  XROSpecificProperty(v8::Handle<v8::ObjectTemplate>, prototype, public, protected);

  XROProperty(const char *, typeName);
  XROProperty(xsize, typeId);

  XROProperty(xsize, typeIdField);
  XROProperty(xsize, nativeField);

public:
  XInterfaceBase(xsize typeID,
                 const char *typeName,
                 XScriptObject ctor(XScriptArguments const &argv),
                 xsize typeIdField,
                 xsize nativeField,
                 xsize internalFieldCount);

  void seal();
  bool isSealed() const
    {
    return _isSealed;
    }

  void wrapInstance(XInterfaceObject obj, void *object) const;
  void unwrapInstance(XInterfaceObject object) const;

  XInterfaceObject newInstance(int argc, v8::Handle<v8::Value> argv[]) const;
  XInterfaceObject newInstanceBase() const;

  v8::Handle<v8::Function> constructorFunction() const;
  void set(const char *name, v8::Handle<v8::Value> val);
  void addProperty(const char *name, v8::AccessorGetter, v8::AccessorSetter);

  void addClassTo(char const *thisClassName, v8::Handle<v8::Object> const &dest) const;

  void inherit(XInterfaceBase* parentType);

protected:
  mutable bool _isSealed;
  };

template <typename T> const XInterfaceBase* findInterface(const T*);

template <typename T> class XInterface : public XInterfaceBase
  {
public:
  typedef typename XPlainType<T>::Type Type;

  void wrapInstance(v8::Handle<v8::Object> object, T *native) const
    {
    XInterfaceBase::wrapInstance(object, (void*)native);
    }

  void unwrapInstance(v8::Handle<v8::Object> object) const
    {
    return XInterfaceBase::unwrapInstance(object);
    }

  template <typename ValueT> inline void set(char const *name, ValueT val)
    {
    set(name, cvv8::CastToJS(val));
    }

  inline void set(char const *name, v8::InvocationCallback val)
    {
    set(name, cvv8::CastToJS(val));
    }

  template <typename GETTYPE,
            typename SETTYPE,
            typename XConstMethodSignature<T, GETTYPE ()>::FunctionType GETTERMETHOD,
            typename XMethodSignature<T, void (SETTYPE)>::FunctionType SETTERMETHOD>
  void addProperty(const char *name)
    {
    v8::AccessorGetter getter = (v8::AccessorGetter)cvv8::XConstMethodToGetter<T, GETTYPE (), GETTERMETHOD>::Get;
    v8::AccessorSetter setter = (v8::AccessorSetter)cvv8::XMethodToSetter<T, SETTYPE, SETTERMETHOD>::Set;

    XInterfaceBase::addProperty(name, getter, setter);
    }

  template <typename GETTYPE,
            typename XMethodSignature<T, GETTYPE ()>::FunctionType GETTERMETHOD>
  void addAccessProperty(const char *name)
    {
    v8::AccessorGetter getter = (v8::AccessorGetter)cvv8::XMethodToGetter<T, GETTYPE (), GETTERMETHOD>::Get;

    XInterfaceBase::addProperty(name, getter, 0);
    }

  /**
     Destroys the given object by disconnecting its associated
     native object and calling the native destructor function
     for it.

     If jo cannot be converted to a T then false is
     returned. Otherwise the true is returned and the native
     object referenced by jo is no longer valid (it should not
     be used by JS code).

     Native functions bound to that object should take care to
     bail out with an exception once the native pointer is gone,
     as opposed to blindly stepping on its null/dangling pointer
     (which _might_ have been re-allocated to a different
     object, even of a different type, in the mean time).
  */
  static bool destroyObject( v8::Handle<v8::Object> const & jo )
  {
      T * t = CastFromJS<T>(jo);
      if( ! t ) return false;
      else
      {
          v8::Persistent<v8::Object> p( v8::Persistent<v8::Object>::New( jo ) );
          p.ClearWeak(); // avoid a second call to weak_dtor() via gc!
          weak_dtor( p, t );
          return true;
      }
  }
  /**
     If jv is empty or !jv->IsObject() then false is returned,
     otherwise it returns the result of
     DestroyObject(Handle<Object>).
  */
  static bool destroyObject( v8::Handle<v8::Value> const & jv )
  {
      return (jv.IsEmpty() || !jv->IsObject())
          ? false
          : destroyObject( v8::Handle<v8::Object>( v8::Object::Cast(*jv) ) );
  }

  /**
     A v8::InvocationCallback implementation which calls
     DestroyObject( argv.This() ).

     It is intended to be used as a "manual destructor" for
     classes which need it. The canonical examples are
     Stream.close() and Database.close().

     This function is not called DestroyObject to avoid name
     collisions during binding using Set(...,DestroyObjectCallback).
  */
  static v8::Handle<v8::Value> destroyObjectCallback( XScriptArguments const & argv )
    {
    return destroyObject(argv.This()) ? v8::True() : v8::False();
    }

  static XInterface *create()
    {
    XInterface &bob = instance();
    xAssert(!bob.isSealed());
    return &bob;
    }

  static const XInterface *lookup()
    {
    const XInterface &bob = instance();
    xAssert(bob.isSealed());
    return &bob;
    }

private:
  typedef cvv8::ClassCreator_InternalFields<T> InternalFields;
  typedef cvv8::ClassCreator_WeakWrap<T> WeakWrap;
  typedef cvv8::ClassCreator_Factory<T> Factory;

  static XInterface &instance()
    {
    static XInterface bob;
    return bob;
    }

  static XInterfaceObject FindHolder( XInterfaceObject const & jo, T const * nh )
  {
      if( !nh || !jo.isValid() ) return XInterfaceObject();
      XScriptObject proto(jo);
      void const * ext = NULL;
      typedef cvv8::ClassCreator_SearchPrototypeForThis<T> SPFT;
      while( !ext && proto.isValid() && proto.isObject() )
      {
          XInterfaceObject obj(proto);
          ext = (obj.internalFieldCount() != InternalFields::Count)
              ? NULL
              : obj.internalField( InternalFields::NativeIndex );
          // FIXME: if InternalFields::TypeIDIndex>=0 then also do a check on that one.
          /*
              If !ext, there is no bound pointer. If (ext &&
              (ext!=nh)) then there is one, but it's not the droid
              we're looking for. In either case, (possibly) check the
              prototype...
          */
          if( ext == nh )
            {
            return obj;
            }
          else if( !SPFT::Value )
            {
            break;
            }
          else
            {
            proto = obj.getPrototype();
            }
      }
      return XInterfaceObject();
  }

  static void weak_dtor( XPersistentScriptObject pv, void * )
  {
      using namespace v8;
      //std::cerr << "Entering weak_dtor<>(native="<<(void const *)nobj<<")\n";
      XInterfaceObject jobj(pv);
      typedef typename XScriptConvert::internal::JSToNative<T>::ResultType NT;
      NT native = XScriptConvert::from<T>( pv );
      if( !native )
      {
          /* see: http://code.google.com/p/v8-juice/issues/detail?id=27

          When i call pv.Dispose(), this function is getting called twice,
          and the second time won't work. i'm going to igore (return w/o
          side-effects) this for now for the sake of avoiding a crash
          which i'm seeing only on 64-bit platforms.

          However, even if i return here, v8 is crashing with a
          !NEAR_DEATH assertion right after the second call is made.

          The extra pair of Dispose()/Clear() calls seems to eliminate that
          crash, but the fact that this code block is hit AT ALL is a
          sign of a problem - the dtor shouldn't be called twice!
          */
          pv.dispose();
          pv.clear();
#if 1 /* i believe this problem was fixed. If you are reading this b/c
   you followed an assert() message, please report this as a bug.
  */
          assert( 0 && "weak_dtor() got no native object!");
#endif
          return;
      }
      else
      {
          /**
             Reminder: the FindHolder() bits are here to
             assist when the bound native exists somewhere in the
             prototype chain other than jobj itself. In that case,
             jobj is valid but we cannot clear out the native handle
             internal field on it because it has no internal fields
             (or none that belong to us).

             To fix this properly we have to be able to know
             _exactly_ which JS object in the prototype chain nh is
             bound to.
          */
          XInterfaceObject nholder = FindHolder( jobj, native );
#if 0 /* reminder: i've never actually seen this error happen, i'm just pedantic about checking... */
          assert( ! nholder.isValid() );
          WeakWrap::Unwrap( nholder /*jobj? subtle difference!*/, native );
          if( nholder.isValid() || (nholder->InternalFieldCount() != InternalFields::Count) )
          {
            xAssertFail();
            /*  cvv8::StringBuffer msg;
              msg << "SERIOUS INTERNAL ERROR:\n"
                  << "ClassCreator<T>::weak_dtor() "
                  << "validated that the JS/Native belong together, but "
                  << "FindHolder() returned an "
                  << (nholder.IsEmpty() ? "empty" : "invalid")
                  << " handle!\n"
                  << "From JS=@"<<(void const *)nobj
                  << ", Converted to Native=@"<<(void const *)native
                  << ", nholder field count="<<nholder->InternalFieldCount()
                  << ", jobj field count="<<jobj->InternalFieldCount()
                  << "\nTHIS MAY LEAD TO A CRASH IF THIS JS HANDLE IS USED AGAIN!!!\n"
                  ;*/
              Factory::Delete(native);
              pv.Dispose(); pv.Clear(); /* see comments below!*/
              Toss("SERIOUS INTERNAL ERROR");
              return;
          }
          else
          {
            findInterface<T>(native)->unwrapInstance(nholder);
            Factory::Delete(native);
          }
#else
          WeakWrap::Unwrap( nholder, native );
          findInterface<T>(native)->unwrapInstance(nholder);
          Factory::Delete(native);
#endif
      }
      /*
        According to the v8 gurus i need to call pv.Dispose()
        instead of pv.Clear(), but if i do then this dtor is
        being called twice. If i don't call it, v8 is crashing
        sometime after this function with a !NEAR_DEATH
        assertion.
      */
      pv.dispose();
      pv.clear();
  }

  /**
     Gets installed as the NewInstance() handler for T.
   */
  static XScriptObject ctor_proxy( XScriptArguments const & argv )
  {
      using namespace v8;
      if(cvv8::ClassCreator_AllowCtorWithoutNew<T>::Value)
      {
          /**
             Allow construction without 'new' by forcing this
             function to be called in a ctor context...
          */
          if (!argv.isConstructCall())
          {
            return argv.callee().callAsConstructor(argv);
          }
      }
      else
      {
          /**
             Why have this limitation? If we don't, v8 pukes
             when the ctor is called, with
             "v8::Object::SetInternalField() Writing internal
             field out of bounds".
          */
          if (!argv.isConstructCall())
          {
            return Toss("This constructor cannot be called as function!");
          }
      }

      /*CastToJS<T>(*nobj)

      We are not yet far enough
      along in the binding that
      CastToJS() can work. And it
      can't work for the generic
      case, anyway.
      */
      XInterfaceObject jobj(argv.calleeThis());
      if( !jobj.isValid() )
        {
        return jobj /* assume exception*/;
        }

      //Persistent<Object> self( Persistent<Object>::New(jobj) );
      T * nobj = NULL;
      try
      {
          WeakWrap::PreWrap( jobj, argv  );
          nobj = Factory::Create( jobj, argv );
          if( ! nobj )
          {
            return XScriptConvert::to<std::exception>(std::runtime_error("Native constructor failed."));
          }
          WeakWrap::Wrap( jobj, nobj );
          jobj.makeWeak( nobj, weak_dtor );
          findInterface<T>(nobj)->wrapInstance(jobj, nobj);
      }
      catch(std::exception const &ex)
      {
          WeakWrap::Unwrap( jobj, nobj );
          if( nobj ) Factory::Delete( nobj );
          jobj = XInterfaceObject();
          return Toss(ex.what());
      }
      catch(...)
      {
          WeakWrap::Unwrap( jobj, nobj );
          if( nobj ) Factory::Delete( nobj );
          jobj = XInterfaceObject();
          return Toss("Native constructor threw an unknown exception!");
      }
      return jobj;
    }

  XInterface() : XInterfaceBase((xsize)cvv8::ClassCreator_TypeID<T>::Value,
                                XScriptTypeInfo<T>::TypeName,
                                ctor_proxy,
                                InternalFields::TypeIDIndex,
                                InternalFields::NativeIndex,
                                InternalFields::Count)
    {
    }
  };

// Intended for specialising.
template <typename T> const XInterfaceBase *findInterface(const T*)
  {
  return XInterface<T>::lookup();
  }

namespace cvv8
{
  template <typename T> struct NativeToJSCopyableType
  {
    XScriptObject operator()(T const *n) const
    {
      T *out = 0;
      const XInterfaceBase* interface = findInterface<T>(n);
      XInterfaceObject obj = interface->newInstance(0, 0);

      out = XScriptConvert::from<T>(obj);
      *out = *n;
      return obj;
    }
    XScriptObject operator()(T const &n) const
    {
      return this->operator()(&n);
    }
  };

  template <typename T> struct NativeToJSConvertableType
  {
    XScriptObject operator()(T *n) const
    {
      const XInterfaceBase* interface = findInterface<T>(n);
      XInterfaceObject self = interface->newInstanceBase();
      interface->wrapInstance(self, n);
      return self;
    }
    XScriptObject operator()(T &n) const
    {
      return this->operator()(&n);
    }
  };

  template <typename T, typename CTORS> class ClassCreatorCopyableFactory
  {
  public:
    typedef T* ReturnType;

    static T *Create(XInterfaceObject &jsSelf, XScriptArguments const & argv)
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

#define X_SCRIPTABLE_CONSTRUCTOR_DEF(variable, type, n) variable,

#define X_SCRIPTABLE_BUILD_CONSTRUCTABLE_TYPEDEF(name, type, ...)  typedef XSignature< type (X_EXPAND_ARGS(X_SCRIPTABLE_CONSTRUCTOR_DEF, type, __VA_ARGS__) cvv8::CtorForwarder<type *(const type &)>, cvv8::CtorForwarder<type *()> )> name;

#define X_SCRIPTABLE_BUILD_CONSTRUCTABLE(type, ...) \
X_SCRIPTABLE_BUILD_CONSTRUCTABLE_TYPEDEF(type##Ctors, type, __VA_ARGS__) \
template <> class ClassCreator_Factory<type> : public ClassCreatorCopyableFactory<type, type##Ctors> {};

#define X_SCRIPTABLE_MATCHERS(type) \
template <> const type& match<const type&, type*>(type *in, bool& valid) { return ptrMatcher<type>(in, valid); }

#define X_SCRIPTABLE_TYPE_BASE(type)  \
  CVV8_TypeInfo_DECL(type); \
  namespace XScriptConvert { namespace internal { \
  template <> struct JSToNative<type> : cvv8::JSToNative_ClassCreator<type, false> {}; } \
  X_SCRIPTABLE_MATCHERS(type) \
  }

#define X_SCRIPTABLE_TYPE_COPYABLE(type, ...) X_SCRIPTABLE_TYPE_BASE(type) \
  namespace XScriptConvert { namespace internal { \
  template <> struct NativeToJS<type> : public cvv8::NativeToJSCopyableType<type> {}; } } \
  namespace cvv8 { \
  X_SCRIPTABLE_BUILD_CONSTRUCTABLE(type, __VA_ARGS__) \
  }

#define X_SCRIPTABLE_TYPE(type, ...) X_SCRIPTABLE_TYPE_BASE(type) \
  namespace XScriptConvert { namespace internal { \
  template <> struct NativeToJS<type> : public cvv8::NativeToJSConvertableType<type> {}; } } \
  namespace cvv8 { \
  X_SCRIPTABLE_BUILD_CONSTRUCTABLE(type, __VA_ARGS__) \
  }

#define X_SCRIPTABLE_TYPE_NOT_COPYABLE(type) X_SCRIPTABLE_TYPE_BASE(type)

#define X_IMPLEMENT_SCRIPTABLE_TYPE_BASE(type, name) \
  CVV8_TypeInfo_IMPL(type, name);

#define X_IMPLEMENT_SCRIPTABLE_TYPE_COPYABLE(type, name) X_IMPLEMENT_SCRIPTABLE_TYPE_BASE(type, name)
#define X_IMPLEMENT_SCRIPTABLE_TYPE(type, name) X_IMPLEMENT_SCRIPTABLE_TYPE_BASE(type, name)

#define X_IMPLEMENT_SCRIPTABLE_TYPE_NOT_COPYABLE(type, name) X_IMPLEMENT_SCRIPTABLE_TYPE_BASE(type, name)
}

#endif // XINTERFACE_H
