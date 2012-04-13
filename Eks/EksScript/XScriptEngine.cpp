#include "XScriptEngine.h"
#include "XScriptValue.h"
#include "XInterface.h"
#include "XConvertToScript.h"
#include "XScriptValueV8Internals.h"
#include "XQObjectWrapper.h"
#include "XQtWrappers.h"
#include "v8.h"
#include "v8-debug.h"

struct StaticEngine
  {
  StaticEngine() : globalTemplate(v8::ObjectTemplate::New()),
      context(v8::Context::New(NULL, globalTemplate)),
      contextScope(context)
    {
    context->AllowCodeGenerationFromStrings(false);
    }

  v8::Locker locker;
  v8::HandleScope scope;
  v8::Handle<v8::ObjectTemplate> globalTemplate;
  v8::Persistent<v8::Context> context;
  v8::Context::Scope contextScope;
  v8::Unlocker unlocker;
  };

StaticEngine *g_engine = 0;

void fatal(const char* location, const char* message)
  {
  qFatal("%s: %s", location, message);
  }

void XScriptEngine::initiate()
  {
  if(g_engine)
    {
    return;
    }

  v8::V8::SetFatalErrorHandler(fatal);

  g_engine = new StaticEngine();
  }

void XScriptEngine::terminate()
  {
  v8::V8::LowMemoryNotification();
  g_engine->context.Dispose();
  delete g_engine;
  }


void debugHandler() {
  // We are in some random thread. We should already have v8::Locker acquired
  // (we requested this when registered this callback). We was called
  // because new debug messages arrived; they may have already been processed,
  // but we shouldn't worry about this.
  //
  // All we have to do is to set context and call ProcessDebugMessages.
  //
  // We should decide which V8 context to use here. This is important for
  // "evaluate" command, because it must be executed some context.
  // In our sample we have only one context, so there is nothing really to
  // think about.

  v8::HandleScope scope;

  //v8::Context::Scope contextScope(g_engine->context);

  //v8::Debug::ProcessDebugMessages();
}

XScriptEngine::XScriptEngine(bool debugging)
  {
  XQObjectWrapper::initiate(this);
  XQtWrappers::initiate(this);

  if(debugging)
    {
    v8::Debug::EnableAgent("XScriptAgent", 9222, false);
    v8::Debug::SetDebugMessageDispatchHandler(debugHandler, true);
    }
  }

XScriptEngine::~XScriptEngine()
  {
  }

XScriptObject XScriptEngine::get(const QString& name)
  {
  XScriptValue propName = XScriptConvert::to(name);
  return fromHandle(g_engine->context->Global()->Get(getV8Internal(propName)));
  }

void XScriptEngine::set(const QString& in, const XScriptObject& obj)
  {
  XScriptValue propName = XScriptConvert::to(in);
  g_engine->context->Global()->Set(getV8Internal(propName), getV8Internal(obj));
  }

void XScriptEngine::set(const QString &name, Function fn)
  {
  XScriptValue propName = XScriptConvert::to(name);
  v8::Handle<v8::FunctionTemplate> fnTmpl = ::v8::FunctionTemplate::New((v8::InvocationCallback)fn);

  g_engine->context->Global()->Set(getV8Internal(propName), fnTmpl->GetFunction());
  }


void XScriptEngine::addInterface(const XInterfaceBase *i)
  {
  xAssert(i->isSealed());
  i->addClassTo(i->typeName(), fromHandle(g_engine->context->Global()));
  }


void XScriptEngine::adjustAmountOfExternalAllocatedMemory(int in)
  {
  v8::V8::AdjustAmountOfExternalAllocatedMemory(in);
  }

v8::Handle<v8::ObjectTemplate> getGlobalTemplate(XScriptEngine *)
  {
  return g_engine->globalTemplate;
  }

v8::Handle<v8::Context> getV8EngineInternal()
  {
  return g_engine->context;
  }
