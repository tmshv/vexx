#include "XScriptSource.h"
#include "XScriptValue.h"
#include "XScriptValueV8Internals.h"
#include "XAssert"

struct XScriptSourceImpl
{
  static XScriptSourceImpl* impl(XScriptSource *s) { return reinterpret_cast<XScriptSourceImpl*>(s); }
  static const XScriptSourceImpl* impl(const XScriptSource *s) { return reinterpret_cast<const XScriptSourceImpl*>(s); }

  v8::Handle<v8::Script> _script;
  };

xCompileTimeAssert(sizeof(XScriptSourceImpl) == sizeof(XScriptSource));

XScriptSource::XScriptSource(const char *data)
  {
  // Create a string containing the JavaScript source code.
  v8::Handle<v8::String> source = v8::String::New(data);

  // Compile the source code.
  XScriptSourceImpl *i = XScriptSourceImpl::impl(this);
  i->_script = v8::Script::Compile(source);
  }

XScriptValue XScriptSource::run(bool *error)
  {
  v8::TryCatch trycatch;
  // Run the script to get the result.
  XScriptSourceImpl *i = XScriptSourceImpl::impl(this);
  v8::Handle<v8::Value> result = i->_script->Run();

  if(error)
  {
    *error = false;
  }

  if (result.IsEmpty())
    {
    if(error)
    {
      *error = true;
    }
    v8::String::AsciiValue exception_str(trycatch.Exception());
    return XScriptValue(*exception_str);
    }

  return fromHandle(result);
  }
