#include "XScriptFunction.h"
#include "XScriptValue.h"
#include "XScriptObject.h"
#include "XScriptException.h"
#include "XScriptValueV8Internals.h"

struct XScriptFunctionInternal
  {
  static XScriptFunctionInternal *init(XScriptFunction *o, const XScriptFunction *other=0)
    {
    XScriptFunctionInternal *internal = (XScriptFunctionInternal*)o;
    new(internal) XScriptFunctionInternal;

    if(other)
      {
      const XScriptFunctionInternal *otherInternal = (const XScriptFunctionInternal*)other;
      internal->_object = otherInternal->_object;
      }

    return internal;
    }

  static void term(XScriptFunction *o)
    {
    XScriptFunctionInternal *internal = (XScriptFunctionInternal*)o;
    (void)internal;
    internal->~XScriptFunctionInternal();
    }

  static const XScriptFunctionInternal *val(const XScriptFunction *o)
    {
    const XScriptFunctionInternal *internal = (const XScriptFunctionInternal*)o;
    return internal;
    }

  static XScriptFunctionInternal *val(XScriptFunction *o)
    {
    XScriptFunctionInternal *internal = (XScriptFunctionInternal*)o;
    return internal;
    }

  mutable v8::Handle<v8::Function> _object;
  };
xCompileTimeAssert(sizeof(XScriptFunction) == sizeof(XScriptFunctionInternal));

struct XScriptArgumentsInternal
  {
  static XScriptArgumentsInternal *init(XScriptFunction *o)
    {
    XScriptArgumentsInternal *internal = (XScriptArgumentsInternal*)o;
    new(internal) XScriptArgumentsInternal;


    return internal;
    }

  static void term(XScriptFunction *o)
    {
    XScriptArgumentsInternal *internal = (XScriptArgumentsInternal*)o;
    (void)internal;
    internal->~XScriptArgumentsInternal();
    }

  static const v8::Arguments &val(const XScriptArguments *o)
    {
    XScriptArgumentsInternal *internal = (XScriptArgumentsInternal*)o;
    return (v8::Arguments&)internal->_args;
    }

  mutable xuint8 _args[sizeof(v8::Arguments)];
  };
xCompileTimeAssert(sizeof(XScriptArguments) == sizeof(XScriptArgumentsInternal));

struct XAccessorInfoInternal
  {
  static XAccessorInfoInternal *init(XAccessorInfo *o)
    {
    XAccessorInfoInternal *internal = (XAccessorInfoInternal*)o;
    new(internal) XAccessorInfoInternal;


    return internal;
    }

  static void term(XAccessorInfo *o)
    {
    XAccessorInfoInternal *internal = (XAccessorInfoInternal*)o;
    (void)internal;
    internal->~XAccessorInfoInternal();
    }

  static const v8::AccessorInfo &val(const XAccessorInfo *o)
    {
    XAccessorInfoInternal *internal = (XAccessorInfoInternal*)o;
    return (v8::AccessorInfo&)internal->_args;
    }

  mutable xuint8 _args[sizeof(v8::AccessorInfo)];
  };
xCompileTimeAssert(sizeof(XAccessorInfo) == sizeof(XAccessorInfoInternal));

XScriptFunction::XScriptFunction()
  {
  XScriptFunctionInternal::init(this);
  }

XScriptFunction::~XScriptFunction()
  {
  XScriptFunctionInternal::term(this);
  }

XScriptFunction::XScriptFunction(const XScriptFunction &other)
  {
  XScriptFunctionInternal::init(this, &other);
  }

XScriptFunction& XScriptFunction::operator=(const XScriptFunction &other)
  {
  XScriptFunctionInternal::term(this);
  XScriptFunctionInternal::init(this, &other);
  return *this;
  }

XScriptValue XScriptFunction::call(const XScriptObject &self, int argc, const XScriptValue args[]) const
  {
  const XScriptFunctionInternal* func = XScriptFunctionInternal::val(this);

  return (func->_object.IsEmpty() || func->_object.IsEmpty())
      ? Toss("Illegal argument: empty v8::Handle<>.")
      : fromHandle(func->_object->Call(getV8Internal(self), argc, getV8Internal(args)));
  }

XScriptValue XScriptFunction::callWithTryCatch(const XScriptObject &self, int argc, const XScriptValue args[], bool *error) const
  {
  v8::TryCatch trycatch;
  if(error)
  {
    *error = false;
  }

  const XScriptFunctionInternal* func = XScriptFunctionInternal::val(this);
  v8::Handle<v8::Value> result = func->_object->Call(getV8Internal(self), argc, getV8Internal(args));

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

XScriptValue XScriptFunction::callAsConstructor(const XScriptArguments &argv)
  {
  const v8::Arguments &args = XScriptArgumentsInternal::val(&argv);

  const int argc = args.Length();
  std::vector< v8::Handle<v8::Value> > av(static_cast<size_t>(argc),v8::Undefined());

  for( int i = 0; i < argc; ++i )
    {
    av[i] = args[i];
    }

  const XScriptFunctionInternal* func = XScriptFunctionInternal::val(this);
  return fromHandle(func->_object->NewInstance(argc, &av[0]));
  }

XScriptFunction fromFunction(v8::Handle<v8::Function> fn)
  {
  XScriptFunction o;
  XScriptFunctionInternal *internal = XScriptFunctionInternal::val(&o);
  internal->_object = fn;
  return o;
  }

XScriptObject XAccessorInfo::calleeThis() const
  {
  return fromObjectHandle(XAccessorInfoInternal::val(this).This());
  }

XScriptArguments::XScriptArguments()
  {
  }

XScriptArguments::~XScriptArguments()
  {
  }

bool XScriptArguments::isConstructCall() const
  {
  return XScriptArgumentsInternal::val(this).IsConstructCall();
  }

XScriptFunction XScriptArguments::callee() const
  {
  return fromFunction(XScriptArgumentsInternal::val(this).Callee());
  }

XScriptObject XScriptArguments::calleeThis() const
  {
  return fromObjectHandle(XScriptArgumentsInternal::val(this).This());
  }

xsize XScriptArguments::length() const
  {
  return XScriptArgumentsInternal::val(this).Length();
  }

XScriptValue XScriptArguments::at(xsize i) const
  {
  return fromHandle(XScriptArgumentsInternal::val(this)[i]);
  }

v8::Handle<v8::Function> getV8Internal(const XScriptFunction &o)
  {
  const XScriptFunctionInternal *internal = XScriptFunctionInternal::val(&o);
  return internal->_object;
  }
