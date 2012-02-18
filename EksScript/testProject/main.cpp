#include "v8.h"
#include "QMetaType"
#include "QHash"

using namespace v8;

#ifndef Q_CC_MSVC
# error not setup for non msvc, needs solid testing in member pointers
#endif

class SomeClass
{
public:
  virtual ~SomeClass() {}

  virtual int getNonStatic()
  {
    return nonStatic;
  }

  virtual void setNonStatic(const int &inA)
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

template <typename T> Handle<Value> Pack(T t);
template <typename T> T Unpack(Handle<Value>);

template <> Handle<Value> Pack(int i)
{
  return Integer::New(i);
}

template <> int Unpack(Handle<Value> v)
{
  return v->Int32Value();
}

struct WrappedStaticProperty
{
  void *get;
  void *set;

  template <typename T>
  static Handle<Value> StaticGetter(Local<String>, const AccessorInfo& info)
  {
    Local<Value> v = info.Data();
    WrappedStaticProperty *p = (WrappedStaticProperty *)External::Unwrap(v);

    typedef T (*GET)() ;
    GET g = (GET)p->get;

    T val = g();

    return Pack<T>(val);
  }

  template <typename T>
  static void StaticSetter(Local<String>, Local<Value> value, const AccessorInfo& info)
  {
    Local<Value> v = info.Data();
    WrappedStaticProperty *p = (WrappedStaticProperty *)External::Unwrap(v);

    typedef void (*SET)(const T &);
    SET g = (SET)p->set;

    g(Unpack<T>(value));
  }
};

struct WrappedProperty
{
  void *get[2];
  void *set[2];

  template <typename T>
  static Handle<Value> Getter(Local<String>, const AccessorInfo& info)
  {
    class CLASS
    {
    };

    Local<Value> v = info.Data();
    WrappedProperty *p = (WrappedProperty *)External::Unwrap(v);

    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();

    typedef T (CLASS::*GET)();
    GET g = *(GET*)p->get;

    T val = (static_cast<CLASS*>(ptr)->*g)();
    return Pack<T>(val);
  }

  template <typename T>
  static void Setter(Local<String>, Local<Value> value, const AccessorInfo& info)
  {
    class CLASS
    {
    };

    Local<Value> v = info.Data();
    WrappedProperty *p = (WrappedProperty *)External::Unwrap(v);

    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
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

void addStaticAccessor(Handle<ObjectTemplate>& tmpl, const char *name, int id, void *get, void *set)
{
  WrappedStaticProperty* wrappedData = new WrappedStaticProperty;
  wrappedData->get = get;
  wrappedData->set = set;

  Handle<Value> data = External::Wrap(wrappedData);

  const TypeWrapper wrap = TypeWrapper::findType(id);
  tmpl->SetAccessor(String::New(name), (v8::AccessorGetter)wrap.staticGet, (v8::AccessorSetter)wrap.staticSet, data);
}

template <typename GETFUNC, typename SETFUNC>
void addAccessor(Handle<ObjectTemplate>& tmpl, const char *name, int id, GETFUNC g,  SETFUNC s)
{
  _ASSERT(sizeof(g) <= (sizeof(void *)*2));
  _ASSERT(sizeof(s) <= (sizeof(void *)*2));

  WrappedProperty* wrappedData = new WrappedProperty;
  memcpy(wrappedData->get, &g, sizeof(void*)*2);
  memcpy(wrappedData->set, &s, sizeof(void*)*2);

  Handle<Value> data = External::Wrap(wrappedData);

  const TypeWrapper wrap = TypeWrapper::findType(id);
  tmpl->SetAccessor(String::New(name), (v8::AccessorGetter)wrap.get, (v8::AccessorSetter)wrap.set, data);
}

class Interface
{

};

int main(int, char*[])
{
  // Create a stack-allocated handle scope.
  HandleScope handle_scope;

  Handle<ObjectTemplate> global_templ = ObjectTemplate::New();

  Interface someTempl;
  someTempl.addStaticProperty<int>("a", SomeClass::getA, SomeClass::setA); 
  someTempl.addProperty<int>("a", &SomeClass::getNonStatic, &SomeClass::setNonStatic); 

  Handle<ObjectTemplate> point_templ = ObjectTemplate::New();
  point_templ->SetInternalFieldCount(1);

  TypeWrapper::wrap<int>();
  addStaticAccessor(point_templ, "a", qMetaTypeId<int>(), SomeClass::getA, SomeClass::setA);
  addAccessor(point_templ, "x", qMetaTypeId<int>(), &SomeClass::getNonStatic, &SomeClass::setNonStatic);

  // Create a new context.
  Persistent<Context> context = Context::New(NULL, global_templ);

  // Enter the created context for compiling and
  // running the hello world script.
  Context::Scope context_scope(context);

  SomeClass c;
  Local<Object> obj = point_templ->NewInstance();
  obj->SetInternalField(0, External::New(&c));

  Handle<String> propName = String::New("someClass");
  context->Global()->Set(propName, obj);

  // Create a string containing the JavaScript source code.
  Handle<String> source = String::New("someClass.a = 5; someClass.x = 1; 'Hello' + someClass.a.toString() + someClass.x.toString();");

  // Compile the source code.
  Handle<Script> script = Script::Compile(source);

  TryCatch trycatch;
  // Run the script to get the result.
  Handle<Value> result = script->Run();

  if (result.IsEmpty()) {  
    Handle<Value> exception = trycatch.Exception();
    String::AsciiValue exception_str(exception);
    printf("Exception: %s\n", *exception_str);
    // ...
  }

  // Dispose the persistent context.
  context.Dispose();

  // Convert the result to an ASCII string and print it.
  String::AsciiValue ascii(result);
  printf("%s\n", *ascii);
  return 0;
}
