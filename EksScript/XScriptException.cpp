#include "XScriptException.h"
#include "XScriptValue.h"
#include "XScriptValueV8Internals.h"

inline XScriptValue TossAsError(const QString &err)
  {
  v8::Handle<v8::String> string = v8::String::New(err.toUtf8().data());
  return fromHandle(v8::ThrowException(v8::Exception::Error(string)));
  }

inline XScriptValue Toss(const QString &err)
  {
  v8::Handle<v8::String> string = v8::String::New(err.toUtf8().data());
  return fromHandle(v8::ThrowException(string));
  }
