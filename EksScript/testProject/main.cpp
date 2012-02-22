#include "v8.h"
#include "QMetaType"
#include "QVariant"
#include "QHash"

#ifndef Q_CC_MSVC
# error not setup for non msvc, needs solid testing in member pointers
#endif

template <typename T> unsigned int XTypeId()
{
  union
  {
    unsigned int (*fn)();
    unsigned int data;
  } u;

  u.fn = &XTypeId<T>;

  return u.data;
}

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

template <> v8::Handle<v8::Value> Pack(QVariant i)
{
  switch(i.type())
  {
  case QVariant::Int: return Pack(i.toInt());
  default: return v8::Handle<v8::Value>();
  }

}

template <> QVariant Unpack(v8::Handle<v8::Value> v)
{
  if(v->IsInt32())
  {
    return QVariant(Unpack<int>(v));
  }
  
  return QVariant();
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
    v8::Local<v8::Value> v = info.Data();
    WrappedPropertyMap *p = (WrappedPropertyMap *)v8::External::Unwrap(v);

    v8::String::AsciiValue propName(property);

    v8::Local<v8::Object> self = info.Holder();
    if(self->InternalFieldCount() && p->get)
    {
      class CLASS
      {
      };

      v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
      void* ptr = wrap->Value();

      typedef QVariant (CLASS::*GET)(const char *);
      GET g = *(GET*)p->get;

      QVariant val = (static_cast<CLASS*>(ptr)->*g)(*propName);
      if(val.isValid())
      {
        return Pack(val);
      }
    }

    if(p->staticGet)
    {
      typedef QVariant (*GET)(const char *);
      GET g = (GET)p->staticGet;

      QVariant val = g(*propName);

      if(val.isValid())
      {
        return Pack(val);
      }
    }

    return v8::Handle<v8::Value>();
  }

  static v8::Handle<v8::Value> Setter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
  {
    v8::Local<v8::Value> v = info.Data();
    WrappedPropertyMap *p = (WrappedPropertyMap *)v8::External::Unwrap(v);

    v8::String::AsciiValue propName(property);

    v8::Local<v8::Object> self = info.Holder();
    if(self->InternalFieldCount() && p->set)
    {
      class CLASS
      {
      };

      v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
      void* ptr = wrap->Value();

      typedef bool (CLASS::*SET)(const char *, const QVariant &);
      SET s = *(SET*)p->set;

      bool set = (static_cast<CLASS*>(ptr)->*s)(*propName, Unpack<QVariant>(value));
      if(set)
      {
        return Getter(property, info);
      }
    }

    if(p->staticSet)
    {
      typedef bool (*SET)(const char *, const QVariant &);
      SET s = (SET)p->staticSet;

      bool set = s(*propName, Unpack<QVariant>(value));

      if(set)
      {
        return Getter(property, info);
      }
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

    typedef void (*SET)(T);
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

    typedef void (CLASS::*SET)(T);
    SET s = *(SET*)p->set;

    T val = Unpack<T>(value);

    (static_cast<CLASS*>(ptr)->*s)(val);
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

    _wrappedTypes.insert(XTypeId<T>(), t);
      
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

  template <typename TGET, typename TSET, typename CLASS>
  void addProperty(const char *name, TGET (CLASS::*g)(), void (CLASS::*s)(TSET))
  {
    _ASSERT(sizeof(g) <= (sizeof(void *)*2));
    _ASSERT(sizeof(s) <= (sizeof(void *)*2));

    void *get[2];
    void *set[2];
    memcpy(get, &g, sizeof(void*)*2);
    memcpy(set, &s, sizeof(void*)*2);

    int getType = XTypeId<TGET>();
    int setType = XTypeId<TSET>();

    addProperty(name, getType, setType, get, set);
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

  template <typename TGET, typename TSET>
  void addStaticProperty(const char *name, TGET g(), void s(TSET))
  {
    void *get = g;
    void *set = s;

    int getType = XTypeId<TGET>();
    int setType = XTypeId<TSET>();

    addStaticProperty(name, getType, setType, get, set);
  }


private: // internal
  void addPropertyMap(void *g[2], void *s[2], void *sG, void *sS)
  {
    WrappedPropertyMap* wrapped = new WrappedPropertyMap;
    memcpy(wrapped->get, g, sizeof(g));
    memcpy(wrapped->set, s, sizeof(s));
    wrapped->staticGet = sG;
    wrapped->staticSet = sS;
    addMember(wrapped);

    v8::Handle<v8::Value> data = v8::External::Wrap(wrapped);

    _template->SetNamedPropertyHandler(WrappedPropertyMap::Getter, WrappedPropertyMap::Setter, 0, 0, 0, data);
  }

  void addStaticProperty(const char *name, int getId, int setId, void *get, void *set)
  {
    WrappedStaticProperty* wrappedData = new WrappedStaticProperty;
    wrappedData->get = get;
    wrappedData->set = set;

    addMember(wrappedData);

    v8::Handle<v8::Value> data = v8::External::Wrap(wrappedData);

    const TypeWrapper wrapGet = TypeWrapper::findType(getId);
    const TypeWrapper wrapSet = TypeWrapper::findType(setId);
    _template->SetAccessor(v8::String::New(name), (v8::AccessorGetter)wrapGet.staticGet, (v8::AccessorSetter)wrapSet.staticSet, data);
  }

  void addProperty(const char *name, int getId, int setId, void *get[2], void *set[2])
  {
    WrappedProperty* wrapped = new WrappedProperty;
    memcpy(wrapped->get, get, sizeof(get));
    memcpy(wrapped->set, set, sizeof(set));

    addMember(wrapped);
    v8::Handle<v8::Value> data = v8::External::Wrap(wrapped);

    const TypeWrapper wrapGet = TypeWrapper::findType(getId);
    const TypeWrapper wrapSet = TypeWrapper::findType(setId);
    _template->SetAccessor(v8::String::New(name), (v8::AccessorGetter)wrapGet.get, (v8::AccessorSetter)wrapSet.set, data);
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


class Vector3D
{
  float x;
  //void setX(### const... &'s)
};


class SomeClass
{
public:
  virtual ~SomeClass() {}

  virtual Vector3D getNonStatic()
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

int main(int, char*[])
{
  Engine engine;

  TypeWrapper::wrap<int>();
  TypeWrapper::wrap<const int &>();
  TypeWrapper::wrap<Vector3D>();
  TypeWrapper::wrap<const Vector3D &>();

  // build the template
  Interface someTempl;
  someTempl.addStaticProperty("x", SomeClass::getA, SomeClass::setA); 
  someTempl.addProperty("_3D", &SomeClass::getNonStatic, &SomeClass::setNonStatic);
  //someTempl.addPropertyMap<&SomeClass::getOther, &SomeClass::setOther, SomeClass::getOtherStatic, SomeClass::setOtherStatic>();

  // build the template
  //Interface vectorTempl;
  //someTempl.addProperty<QVector3D>("x", &QVector3D::x, &QVector3D::setX);

  // Create a new context.
  Context c(&engine);

  // Enter the created context for compiling and
  // running the hello world script.

  SomeClass wrapable;
  Object obj(&someTempl, &wrapable);

  c.set("someClass", obj);

  Script script("'Hello' + someClass._3D.x");

  script.run();
  return 0;
}
