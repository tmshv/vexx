#include "v8.h"

using namespace v8;

class SomeClass
{
public:
  static int getA()
  {
    return a;
  }

  static void setA(const int &inA)
  {
    a = inA;
  }

  static int a;
};

struct WrappedStaticProperty
{
  Handle<Value> (*get)(const WrappedStaticProperty *);
  void (*set)(const WrappedStaticProperty *, const Local<Value>& val);
  T (*getT)();
  void (*setT)(const T& val);

  template <typename T>
  static WrappedStaticProperty wrap(T get(), void set(const T&))
  {
    WrappedStaticProperty a = { getHelper<T>, setHelper<T>, get, set };

    return a;
  }

  static Handle<Value> Getter(Local<String>, const AccessorInfo& info)
  {
    Local<Value> v = info.Data();
    WrappedStaticProperty *p = (WrappedStaticProperty *)External::Unwrap(v);
    return p->get(p);
  }

  static void Setter(Local<String>, Local<Value> value, const AccessorInfo& info)
  {
    Local<Value> v = info.Data();
    WrappedStaticProperty *p = (WrappedStaticProperty *)External::Unwrap(v);
    return p->set(p, value);
  }

};


int main(int argc, char* argv[]) {

  Handle<ObjectTemplate> global_templ = ObjectTemplate::New();

  WrappedStaticProperty stat = WrappedStaticProperty::wrap(SomeClass::getA, SomeClass::setA);

  Handle<Value> data = External::Wrap(&stat);
  global_templ->SetAccessor(String::New("a"), WrappedStaticProperty::Getter, WrappedStaticProperty::Setter, data);

  // Create a stack-allocated handle scope.
  HandleScope handle_scope;

  // Create a new context.
  Persistent<Context> context = Context::New(NULL, global_templ);

  // Enter the created context for compiling and
  // running the hello world script.
  Context::Scope context_scope(context);

  // Create a string containing the JavaScript source code.
  Handle<String> source = String::New("'Hello' + a.toString()");

  // Compile the source code.
  Handle<Script> script = Script::Compile(source);

  // Run the script to get the result.
  Handle<Value> result = script->Run();

  // Dispose the persistent context.
  context.Dispose();

  // Convert the result to an ASCII string and print it.
  String::AsciiValue ascii(result);
  printf("%s\n", *ascii);
  return 0;
}
