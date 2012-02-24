#include "v8.h"
#include "QMetaType"
#include "QVariant"
#include "QHash"
#include "cvv8/v8-convert.hpp"

template <typename T> class Interface
  {
public:
  Interface()
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

class InterfaceObject
  {
public:
  InterfaceObject()
    {
    }

  template <typename T>
  static InterfaceObject newInstance(Interface<T>*, T** m)
    {
    typedef cvv8::ClassCreator<T> CC;
    InterfaceObject ret;
    ret._object = CC::Instance().NewInstance( 0, NULL, *m );
    return ret;
    }

private:
  v8::Handle<v8::Object> _object;
  friend class Context;
  };

class Engine
  {
public:
  Engine() : _globalTemplate(v8::ObjectTemplate::New())
  {
  }
    
private:
  v8::HandleScope _scope;
  v8::Handle<v8::ObjectTemplate> _globalTemplate;
  friend class Context;
  };

class Context
  {
public:
  Context(Engine *e)
    : _engine(e),
    _context(v8::Context::New(NULL, _engine->_globalTemplate)),
    _scope(_context)
    {
    }

  ~Context()
    {
    // Dispose the persistent context.
    _context.Dispose();
    }

  template <typename T>
  void addInterface(const Interface<T>&)
    {
    typedef cvv8::ClassCreator<T> CC;
    CC& cc(CC::Instance());

    cc.AddClassTo( cvv8::TypeName<T>::Value, _context->Global() );
    }

  void set(const char* in, const InterfaceObject& obj)
    {
      v8::Handle<v8::String> propName = v8::String::New(in);
    _context->Global()->Set(propName, obj._object);
    }

private:
  Engine* _engine;
  v8::Persistent<v8::Context> _context;
  v8::Context::Scope _scope;
  };

class Script
  {
public:
  Script(const char *data)
    {
    // Create a string containing the JavaScript source code.
    v8::Handle<v8::String> source = v8::String::New(data);

    // Compile the source code.
    _script = v8::Script::Compile(source);
    }

  void run()
    {
    v8::TryCatch trycatch;
    // Run the script to get the result.
    v8::Handle<v8::Value> result = _script->Run();

    if (result.IsEmpty())
      {  
      v8::Handle<v8::Value> exception = trycatch.Exception();
      v8::String::AsciiValue exception_str(exception);
      printf("Exception: %s\n", *exception_str);
      // ...
      }

    // Convert the result to an ASCII string and print it.
    v8::String::AsciiValue ascii(result);
    printf("%s\n", *ascii);
    }

private:
  v8::Handle<v8::Script> _script;
  };


class Vector3D
{
public:
  Vector3D()
  {
    x = 0;
  }
  Vector3D(float a)
  {
    x = a;
  }

  float getX() const
  {
    return x;
  }

  void setX(float X)
  {
    x = X;
  }

  float x;
  //void setX(### const... &'s)
};


class SomeClass
{
public:
  SomeClass() : v(0)
  {

  }
  virtual ~SomeClass() {}

  virtual const Vector3D &getNonStatic() const
  {
    return v;
  }

  virtual void setNonStatic(const Vector3D &inA)
  {
    v = inA;
  }

  QVariant getOther(const char *name)
  {
    if(strcmp(name, "other") == 0)
    {
      return other;
    }
    return QVariant();
  }

  virtual bool setOther(const char *name, const QVariant &inA)
  {
    if(strcmp(name, "other") == 0)
    {
      other = inA.toInt();
      return true;
    }
    return false;
  }

  static QVariant getOtherStatic(const char *name)
  {
    if(strcmp(name, "otherStatic") == 0)
    {
      return otherStatic;
    }
    return QVariant();
  }

  static bool setOtherStatic(const char *name, const QVariant &inA)
  {
    if(strcmp(name, "otherStatic") == 0)
    {
      otherStatic = inA.toInt();
      return true;
    }
    return false;
  }

  static int getA()
  {
    return a;
  }

  static void setA(const int &inA)
  {
    a = inA;
  }

  int nonStatic;
  int other;
  static int a;
  static int otherStatic;
  Vector3D v;
};
int SomeClass::a = 0;
int SomeClass::otherStatic = 0;

//-----------------------------------
// Policies used by cv::ClassCreator (it also has others)...
namespace cvv8 {

  // Optional: used mostly for error reporting purposes but can
  // also be used to hold the class' JS-side name (which often differs
  // from its native name).
  CVV8_TypeName_DECL((SomeClass));
  // Out-of-line definition of TypeName<MyType>::Value. The declaration
  // should be visible to any client code which will use the bindings,
  // but the definition may only be done once (put it in the binding's
  // implementation file).
  CVV8_TypeName_IMPL((SomeClass), "SomeClass");

  // The policy which tells ClassCreator how to instantiate and
  // destroy native objects. This one forwards to one of the
  // ctors specified in MyType::Ctors.
  /*typedef Signature<SomeClass (
    CtorForwarder<SomeClass *()>,
    CtorForwarder<SomeClass *(char const *)>,
    CtorForwarder<SomeClass *( int, double )>,
    CtorForwarder<SomeClass *( v8::Arguments const &)>
    )> Ctors;
  template <>
  class ClassCreator_Factory<SomeClass>
    : public ClassCreator_Factory_Dispatcher< SomeClass,
    CtorArityDispatcher<Ctors> >
  {};*/

  // A JSToNative specialization which makes use of the plumbing
  // installed by ClassCreator. This is required so that
  // CastFromJS<MyType>() will work, as the JS/native binding process
  // requires that we be able to convert (via CastFromJS()) a JS-side
  // MyType object to a C++-side MyType object (so that we know what
  // native object to apply a given method/property to).
  template <>
  struct JSToNative< SomeClass > : JSToNative_ClassCreator< SomeClass >
  {};

  

  // There are several other policies not shown here.
  // e.g. ClassCreator_InternalFields defines the v8-side internal
  // field layout (it is rarely necessary to customized that, but
  // it has some obscure uses). We can also hook into the post-construction
  // and pre-destruction phases to perform custom un/binding work if needed.
}

//-----------------------------------
// Policies used by cv::ClassCreator (it also has others)...
namespace cvv8 {

  // Optional: used mostly for error reporting purposes but can
  // also be used to hold the class' JS-side name (which often differs
  // from its native name).
  CVV8_TypeName_DECL((Vector3D));
  // Out-of-line definition of TypeName<MyType>::Value. The declaration
  // should be visible to any client code which will use the bindings,
  // but the definition may only be done once (put it in the binding's
  // implementation file).
  CVV8_TypeName_IMPL((Vector3D), "Vector3D");

  // The policy which tells ClassCreator how to instantiate and
  // destroy native objects. This one forwards to one of the
  // ctors specified in MyType::Ctors.
  /*typedef Signature<SomeClass (
    CtorForwarder<SomeClass *()>,
    CtorForwarder<SomeClass *(char const *)>,
    CtorForwarder<SomeClass *( int, double )>,
    CtorForwarder<SomeClass *( v8::Arguments const &)>
    )> Ctors;
  template <>
  class ClassCreator_Factory<SomeClass>
    : public ClassCreator_Factory_Dispatcher< SomeClass,
    CtorArityDispatcher<Ctors> >
    {};*/


  // A JSToNative specialization which makes use of the plumbing
  // installed by ClassCreator. This is required so that
  // CastFromJS<MyType>() will work, as the JS/native binding process
  // requires that we be able to convert (via CastFromJS()) a JS-side
  // MyType object to a C++-side MyType object (so that we know what
  // native object to apply a given method/property to).
  template <>
  struct JSToNative< Vector3D > : JSToNative_ClassCreator< Vector3D >
  {};

  template <> const Vector3D& Match<const Vector3D&, Vector3D*>(Vector3D *in, bool& valid)
  {
    if(!in)
    {
      valid = false;
    }
    return *in;
  }

  typedef cvv8::Signature<Vector3D (
    cvv8::CtorForwarder<Vector3D *(float)>,
    cvv8::CtorForwarder<Vector3D *()>
    )> Ctors;
  Vector3D *ClassCreator_Factory<Vector3D>::Create( v8::Persistent<v8::Object> & jsSelf, v8::Arguments const & argv )
  {
    typedef cvv8::CtorArityDispatcher<Ctors> Proxy;
    Vector3D *b = Proxy::Call( argv );
    if( b )
    {
      NativeToJSMap<Vector3D>::Insert( jsSelf, b );
    }
    return b;
  }
  void ClassCreator_Factory<Vector3D>::Delete( Vector3D * obj )
  {
    NativeToJSMap<Vector3D>::Remove( obj );
    delete obj;
  }

  template <> struct NativeToJS<Vector3D>
  {
    v8::Handle<v8::Value> operator()( Vector3D const * n ) const
    {
      Vector3D *out = 0;
      v8::Handle<v8::Object> obj = ClassCreator<Vector3D>::Instance().NewInstance( 0, 0, out );     
      *out = *n;
      return obj;
    }
    v8::Handle<v8::Value> operator()( Vector3D const & n ) const
    {
      return this->operator()( &n );
    }
  };

  // There are several other policies not shown here.
  // e.g. ClassCreator_InternalFields defines the v8-side internal
  // field layout (it is rarely necessary to customized that, but
  // it has some obscure uses). We can also hook into the post-construction
  // and pre-destruction phases to perform custom un/binding work if needed.
}

int main(int, char*[])
{
  Engine engine;

  // build the template
  Interface<SomeClass> someTempl;
  someTempl.addProperty<const Vector3D &, const Vector3D &, &SomeClass::getNonStatic, &SomeClass::setNonStatic>("nonStatic");
  //someTempl.addPropertyMap<&SomeClass::getOther, &SomeClass::setOther, SomeClass::getOtherStatic, SomeClass::setOtherStatic>();

  Interface<Vector3D> vecTempl;
  vecTempl.addProperty<float, float, &Vector3D::getX, &Vector3D::setX>("x");

  // Create a new context.
  Context c(&engine);
  c.addInterface(someTempl);
  c.addInterface(vecTempl);

  SomeClass *m = NULL;
  InterfaceObject obj = InterfaceObject::newInstance(&someTempl, &m);

  c.set("someClass", obj);

  Script script("someClass.nonStatic = new Vector3D(5);"
                "someClass.nonStatic.x = 61;"
                "'Hello' + someClass.nonStatic + \" \" + someClass.nonStatic.x;");

  script.run();

  return 0;
}
