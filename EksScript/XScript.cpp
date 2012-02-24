#include "XScript.h"
#include "XAssert"
#include "v8.h"

struct XScriptImpl
{
  static XScriptImpl* impl(XScript *s) { return reinterpret_cast<XScriptImpl*>(s); }
  static const XScriptImpl* impl(const XScript *s) { return reinterpret_cast<const XScriptImpl*>(s); }

  v8::Handle<v8::Script> _script;
  };

xCompileTimeAssert(sizeof(XScriptImpl) == sizeof(XScript));

XScript::XScript(const char *data)
  {
  // Create a string containing the JavaScript source code.
  v8::Handle<v8::String> source = v8::String::New(data);

  // Compile the source code.
  XScriptImpl *i = XScriptImpl::impl(this);
  i->_script = v8::Script::Compile(source);
  }

void XScript::run()
  {
  v8::TryCatch trycatch;
  // Run the script to get the result.
  XScriptImpl *i = XScriptImpl::impl(this);
  v8::Handle<v8::Value> result = i->_script->Run();

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
