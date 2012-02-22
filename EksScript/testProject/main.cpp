#include "v8.h"
#include "QMetaType"
#include "QVariant"
#include "QHash"

#ifndef Q_CC_MSVC
# error not setup for non msvc, needs solid testing in member pointers
#endif

template <typename T> v8::Handle<v8::Value> Pack(T t);
template <typename T> T Unpack(v8::Handle<v8::Value>);

template <> v8::Handle<v8::Value> Pack(int i)
{
  return v8::Integer::New(i);
}

template <> int Unpack(v8::Handle<v8::Value> v)
{
  return v->Int32Value();
}

class WrappedMember
{

};

struct WrappedPropertyMap : public WrappedMember
{
  void *get[2];
  void *set[2];
  void *staticGet;
  void *staticSet;

  static v8::Handle<v8::Value> Getter(v8::Local<v8::String> property, const v8::AccessorInfo& info)
  {
    (void)property;
    (void)info;
    return v8::Handle<v8::Value>();
  }

  static v8::Handle<v8::Value> Setter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
  {
    v8::Local<v8::Value> v = info.Data();
    WrappedPropertyMap *p = (WrappedPropertyMap *)v8::External::Unwrap(v);

    v8::Local<v8::Object> self = info.Holder();
    if(self->InternalFieldCount() && p->set)
    {
      class CLASS
      {
      };

      v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
      void* ptr = wrap->Value();

      typedef QVariant (CLASS::*GET)();
      GET g = *(GET*)p->get;

      QVariant val = (static_cast<CLASS*>(ptr)->*g)();
    }



    return v8::Handle<v8::Value>();
  }
};

struct WrappedStaticProperty : public WrappedMember
{
  void *get;
  void *set;

  template <typename T>
  static v8::Handle<v8::Value> StaticGetter(v8::Local<v8::String>, const v8::AccessorInfo& info)
  {
    v8::Local<v8::Value> v = info.Data();
    WrappedStaticProperty *p = (WrappedStaticProperty *)v8::External::Unwrap(v);

    typedef T (*GET)();
    GET g = (GET)p->get;

    T val = g();

    return Pack<T>(val);
  }

  template <typename T>
  static void StaticSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
  {
    v8::Local<v8::Value> v = info.Data();
    WrappedStaticProperty *p = (WrappedStaticProperty *)v8::External::Unwrap(v);

    typedef void (*SET)(const T &);
    SET g = (SET)p->set;

    g(Unpack<T>(value));
  }
};

struct WrappedProperty : public WrappedMember
{
  void *get[2];
  void *set[2];

  template <typename T>
  static v8::Handle<v8::Value> Getter(v8::Local<v8::String>, const v8::AccessorInfo& info)
  {
    class CLASS
    {
    };

    v8::Local<v8::Value> v = info.Data();
    WrappedProperty *p = (WrappedProperty *)v8::External::Unwrap(v);

    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();

    typedef T (CLASS::*GET)();
    GET g = *(GET*)p->get;

    T val = (static_cast<CLASS*>(ptr)->*g)();
    return Pack<T>(val);
  }

  template <typename T>
  static void Setter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
  {
    class CLASS
    {
    };

    v8::Local<v8::Value> v = info.Data();
    WrappedProperty *p = (WrappedProperty *)v8::External::Unwrap(v);

    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();

    typedef void (CLASS::*SET)(const T &);
    SET s = *(SET*)p->set;

    (static_cast<CLASS*>(ptr)->*s)(Unpack<T>(value));
  }
};

class TypeWrapper
{
public:
  void *staticGet;
  void *staticSet;
  void *get;
  void *set;

  template <typename T> static void wrap()
  {
    v8::AccessorGetter sG = WrappedStaticProperty::StaticGetter<T>;
    v8::AccessorSetter sS = WrappedStaticProperty::StaticSetter<T>;
    v8::AccessorGetter g = WrappedProperty::Getter<T>;
    v8::AccessorSetter s = WrappedProperty::Setter<T>;
    TypeWrapper t = { (void*)sG, (void*)sS, g, s };
    _wrappedTypes.insert(QMetaTypeId2<T>::qt_metatype_id(), t);
      
  }
  static const TypeWrapper findType(int i)
  {
    _ASSERT(_wrappedTypes.contains(i));
    return _wrappedTypes[i];
  }

  static QHash<int, TypeWrapper> _wrappedTypes;
};
QHash<int, TypeWrapper> TypeWrapper::_wrappedTypes;

class Interface
{
public: // external
  Interface() : _template(v8::ObjectTemplate::New())
  {
    _template->SetInternalFieldCount(1);
  }

  template <typename T, typename GETFUNC, typename SETFUNC>
  void addProperty(const char *name, GETFUNC g,  SETFUNC s)
  {
    _ASSERT(sizeof(g) <= (sizeof(void *)*2));
    _ASSERT(sizeof(s) <= (sizeof(void *)*2));

    void *get[2];
    void *set[2];
    memcpy(get, &g, sizeof(void*)*2);
    memcpy(set, &s, sizeof(void*)*2);

    addProperty(name, qMetaTypeId<T>(), get, set);
  }

  template <typename GETFUNC, typename SETFUNC>
  void addPropertyMap(GETFUNC g, SETFUNC s=0, void *sG=0, void *sS=0)
  {
    _ASSERT(sizeof(g) <= (sizeof(void *)*2));
    _ASSERT(sizeof(s) <= (sizeof(void *)*2));
    void *get[2];
    void *set[2];
    memcpy(get, &g, sizeof(void*)*2);
    memcpy(set, &s, sizeof(void*)*2);

    addPropertyMap(get, set, sG, sS);
  }

  /*void addPropertyArray(WrappedPropertyMap::Getter g, WrappedPropertyMap::Setter s=0, WrappedPropertyMap::StaticGetter sG=0, , WrappedPropertyMap::StaticGetter sS=0)
  {
    WrappedPropertyMap* wrappedData = new WrappedPropertyMap;
    wrappedData->get = g;
    wrappedData->set = s;
    wrappedData->staticGet = sG;
    wrappedData->staticSet = sS;
    addMember(wrappedData);

    v8::Handle<v8::Value> data = v8::External::Wrap(wrappedData);

    _template->SetIndexedPropertyHandler(WrappedPropertyArray::GetHelper, WrappedPropertyArray::SetHelper, 0, 0, 0, data);
  }*/

  template <typename T>
  void addStaticProperty(const char *name, void *get, void *set)
  {
    addStaticProperty(name, qMetaTypeId<T>(), get, set);
  }

private: // internal
  void addPropertyMap(void *g[2], void *s[2], void *sG, void *sS)
  {
    WrappedPropertyMap* wrapped = new WrappedPropertyMap;
    memcpy(wrapped->get, g, sizeof(g));
    memcpy(wrapped->set, g, sizeof(s));
    wrapped->staticGet = sG;
    wrapped->staticSet = sS;
    addMember(wrapped);

    v8::Handle<v8::Value> data = v8::External::Wrap(wrapped);

    _template->SetNamedPropertyHandler(WrappedPropertyMap::Getter, WrappedPropertyMap::Setter, 0, 0, 0, data);
  }

  void addStaticProperty(const char *name, int id, void *get, void *set)
  {
    WrappedStaticProperty* wrappedData = new WrappedStaticProperty;
    wrappedData->get = get;
    wrappedData->set = set;

    addMember(wrappedData);

    v8::Handle<v8::Value> data = v8::External::Wrap(wrappedData);

    const TypeWrapper wrap = TypeWrapper::findType(id);
    _template->SetAccessor(v8::String::New(name), (v8::AccessorGetter)wrap.staticGet, (v8::AccessorSetter)wrap.staticSet, data);
  }

  void addProperty(const char *name, int id, void *get[2], void *set[2])
  {
    WrappedProperty* wrapped = new WrappedProperty;
    memcpy(wrapped->get, get, sizeof(get));
    memcpy(wrapped->set, set, sizeof(set));

    addMember(wrapped);
    v8::Handle<v8::Value> data = v8::External::Wrap(wrapped);

    const TypeWrapper wrap = TypeWrapper::findType(id);
    _template->SetAccessor(v8::String::New(name), (v8::AccessorGetter)wrap.get, (v8::AccessorSetter)wrap.set, data);
  }

private:
  void addMember(WrappedMember* )
    {
    // keep list and delete them later...
    }

  v8::Local<v8::ObjectTemplate> _template;
  friend class Object;
};

class Object
  {
public:
  Object(const Interface* ifc, void *thisPtr)
    : _interface(ifc),
    _object(_interface->_template->NewInstance())
  {
    _object->SetInternalField(0, v8::External::New(thisPtr));
  }

private:
  const Interface *_interface;
  v8::Local<v8::Object> _object;
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

  void set(const char* in, const Object& obj)
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





class SomeClass
{
public:
  virtual ~SomeClass() {}

  virtual int getNonStatic()
  {
    return nonStatic;
  }

  virtual void setNonStatic(int inA)
  {
    nonStatic = inA;
  }

  QVariant getOther(const char *, const QVariant &v)
  {
    return nonStatic;
  }

  virtual void setOther(const int &inA)
  {
    nonStatic = inA;
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
  static int a;
};
int SomeClass::a = 0;

int main(int, char*[])
{
  Engine engine;

  TypeWrapper::wrap<int>();

  // build the template
  Interface someTempl;
  someTempl.addStaticProperty<int>("a", SomeClass::getA, SomeClass::setA); 
  someTempl.addProperty<int>("a", &SomeClass::getNonStatic, &SomeClass::setNonStatic);
  someTempl.addPropertyMap(&SomeClass::getOther, &SomeClass::setOther);

  // Create a new context.
  Context c(&engine);

  // Enter the created context for compiling and
  // running the hello world script.

  SomeClass wrapable;
  Object obj(&someTempl, &wrapable);

  c.set("someClass", obj);

  Script script("someClass.a = 5; someClass.x = 1; 'Hello' + someClass.a.toString() + someClass.x.toString();");

  script.run();
  return 0;
}
