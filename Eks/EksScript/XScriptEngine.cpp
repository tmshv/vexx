#include "XScriptValueDartInternals.h"
#include "XScriptValueV8Internals.h"

#ifndef X_DART
# include "v8-debug.h"
#endif

#include "XScriptGlobal.h"
#include "XScriptEngine.h"
#include "XScriptValue.h"
#include "XInterface.h"
#include "XConvertToScript.h"
#include "XQObjectWrapper.h"
#include "XQtWrappers.h"

#ifdef X_DART
bool isolateCreateCallback(const char* ,
  void* ,
  char** )
{
  return true;
}

bool isolateInterruptCallback()
{
  return true;
}

std::map<std::pair<std::string, uint8_t>, Dart_NativeFunction> _symbols;
Dart_Handle buildLibrarySource(const XInterface* ifc)
  {
  }

Dart_NativeFunction Resolve(Dart_Handle name, int num_of_arguments)
{
  const char* cname;
  Dart_StringToCString(name, &cname);

  auto toFind(std::pair<std::string, uint8_t>(cname, num_of_arguments));

  xAssert(_symbols.find(toFind) != _lookup.end());
  return _symbols[toFind];
}

Dart_Handle loadLibrary(Dart_Handle url, Dart_Handle libSrc, Dart_Handle importMap)
{
  Dart_Handle lib = Dart_LoadLibrary(url, libSrc, importMap);

  CHECK_HANDLE(lib)
  Dart_SetNativeResolver(lib, Resolve);

  const int kNumEventHandlerFields = 1;
  Dart_CreateNativeWrapperClass(lib,
    Dart_NewString("NativeWrapper"),
    kNumEventHandlerFields);

  return lib;
}

std::map<std::string, Dart_Handle> _libs;
Dart_Handle tagHandler(Dart_LibraryTag tag, Dart_Handle library, Dart_Handle url, Dart_Handle import_map)
{
  if (!Dart_IsLibrary(library)) {
    return Dart_Error("not a library");
  }
  if (!Dart_IsString8(url)) {
    return Dart_Error("url is not a string");
  }

  if (tag == kCanonicalizeUrl) {
    return url;
  }

  Dart_Handle importLibrary = Dart_LookupLibrary(url);
  if (Dart_IsError(importLibrary))
  {
    const char* url_chars = NULL;
    Dart_Handle result = Dart_StringToCString(url, &url_chars);
    if (Dart_IsError(result)) {
      return Dart_Error("accessing url characters failed");
    }
    std::string strUrl(url_chars);
    xAssert(_libs.find(strUrl) != _libs.end());
    Dart_Handle source = _libs[strUrl];
    importLibrary = loadLibrary(url, source, import_map);
  }

  if (!Dart_IsError(importLibrary) && (tag == kImportTag))
  {
    Dart_Handle result = Dart_LibraryImportLibrary(library, importLibrary);
    CHECK_HANDLE(result);
  }

  return importLibrary;
}

#else

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
#endif

struct StaticEngine
  {
    StaticEngine()
#ifndef X_DART
       : globalTemplate(v8::ObjectTemplate::New()),
      context(v8::Context::New(NULL, globalTemplate)),
      contextScope(context)
#endif
    {
#ifdef X_DART
        Dart_SetVMFlags(0, (const char**)0);
        Dart_Initialize(isolateCreateCallback, isolateInterruptCallback);

        // Start an Isolate, load a script and create a full snapshot.
        Dart_CreateIsolate(0, 0, 0, 0);

        Dart_SetLibraryTagHandler(tagHandler);
#else
        context->AllowCodeGenerationFromStrings(false);
#endif
    }

#ifndef X_DART
  v8::Locker locker;
  v8::HandleScope scope;
  v8::Handle<v8::ObjectTemplate> globalTemplate;
  v8::Persistent<v8::Context> context;
  v8::Context::Scope contextScope;
  v8::Unlocker unlocker;
#endif
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

  g_engine = new StaticEngine();
  }

void XScriptEngine::terminate()
{
#ifdef X_DART
  Dart_ShutdownIsolate();
#else
  v8::V8::LowMemoryNotification();
  g_engine->context.Dispose();
#endif

  delete g_engine;
  }

XScriptEngine::XScriptEngine(bool debugging)
  {
  XQObjectWrapper::initiate(this);
  XQtWrappers::initiate(this);

#ifdef X_DART
  xAssert(!debugging);
#else

  v8::V8::SetFatalErrorHandler(fatal);
  if(debugging)
    {
    v8::Debug::EnableAgent("XScriptAgent", 9222, false);
    v8::Debug::SetDebugMessageDispatchHandler(debugHandler, true);
    }
#endif
  }

XScriptEngine::~XScriptEngine()
  {
  }

#ifndef X_DART
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
#endif


void XScriptEngine::addInterface(const XInterfaceBase *i)
  {
#ifdef X_DART
  _libs[i->typeName()] = buildLibrarySource(i, _symbols);
#else
  xAssert(i->isSealed());
  i->addClassTo(i->typeName(), fromHandle(g_engine->context->Global()));
#endif
  }

void XScriptEngine::adjustAmountOfExternalAllocatedMemory(int in)
  {
#ifndef X_DART
  v8::V8::AdjustAmountOfExternalAllocatedMemory(in);
#endif
  }

#ifndef X_DART
v8::Handle<v8::ObjectTemplate> getGlobalTemplate(XScriptEngine *)
  {
  return g_engine->globalTemplate;
  }

v8::Handle<v8::Context> getV8EngineInternal()
  {
  return g_engine->context;
  }
#endif
