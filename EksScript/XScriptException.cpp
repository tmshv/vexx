#include "XScriptException.h"
#include "XScriptObject.h"
#include "v8.h"

inline XScriptObject TossAsError(const QString &err)
  {
  v8::Handle<v8::String> string = v8::String::New(err.toUtf8().data());
  return v8::ThrowException(v8::Exception::Error(string));
  }

inline XScriptObject Toss(const QString &err)
  {
  v8::Handle<v8::String> string = v8::String::New(err.toUtf8().data());
  return v8::ThrowException(string);
  }
