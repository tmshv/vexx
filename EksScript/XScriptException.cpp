#include "XScriptException.h"
#include "v8.h"

inline v8::Handle<v8::Value> TossAsError(const QString &err)
  {
  v8::Handle<v8::String> string = v8::String::New(err.toUtf8().data());
  return v8::ThrowException(v8::Exception::Error(string));
  }

inline v8::Handle<v8::Value> Toss(const QString &err)
  {
  v8::Handle<v8::String> string = v8::String::New(err.toUtf8().data());
  return v8::ThrowException(string);
  }
