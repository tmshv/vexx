#include "XScriptException.h"
#include "v8.h"

inline v8::Handle<v8::Value> TossAsError(const QString &err)
  {
  return v8::ThrowException(v8::Exception::Error(err->ToString()));
  }

inline v8::Handle<v8::Value> Toss(const QString &err)
  {
  return v8::ThrowException(err);
  }
