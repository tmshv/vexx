#define _ITERATOR_DEBUG_LEVEL 0

#include "dart_api.h"
#include "DartUtils.h"
#include <map>
#include <string>
#include <iostream>

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

struct Nummer
{
  double num;
};

void dtor(Dart_Handle, void *peer)
{
  Nummer *num = (Nummer*)peer;
  delete num;
}

void ctor(Dart_NativeArguments arguments)
{
  int argsC = Dart_GetNativeArgumentCount(arguments);
  Dart_Handle ths = Dart_GetNativeArgument(arguments, 0);
  Dart_Handle arg = Dart_GetNativeArgument(arguments, 1);

  Nummer* num = new Nummer;
  Dart_NewWeakPersistentHandle(ths, num, dtor);

  Dart_DoubleValue(arg, &num->num);

  Dart_SetNativeInstanceField(ths, 0, (intptr_t)num);
}

void crapSalad(Dart_NativeArguments arguments)
{
  int argsC = Dart_GetNativeArgumentCount(arguments);
  Dart_Handle ths = Dart_GetNativeArgument(arguments, 0);
  Dart_Handle arg = Dart_GetNativeArgument(arguments, 1);

  intptr_t thsN = 0;
  Dart_GetNativeInstanceField(ths, 0, &thsN);
  Nummer* num = (Nummer*)thsN;

  double newNum;
  Dart_DoubleValue(arg, &newNum);
  num->num += newNum;

  Dart_Handle str = Dart_ToString(arg);

  Dart_SetReturnValue(arguments, Dart_NewDouble(num->num));
}

std::map<std::pair<std::string, uint8_t>, Dart_NativeFunction> _lookup;
Dart_NativeFunction Resolve(Dart_Handle name, int num_of_arguments)
{ 
  const char* cname; 
  Dart_StringToCString(name, &cname); 

  return _lookup[std::pair<std::string, uint8_t>(std::string(cname), num_of_arguments)];
}

Dart_Handle loadLibrary(Dart_Handle url, Dart_Handle libSrc, Dart_Handle importMap)
{
  Dart_Handle lib = Dart_LoadLibrary(url, libSrc, importMap);

  if(Dart_IsError(lib))
  {
    const char *err = Dart_GetError(lib);
    if(err)
    {
      std::cerr << err << std::endl;
    }
  }
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

    Dart_Handle source = _libs[url_chars];
    importLibrary = loadLibrary(url, source, import_map);
  }

  if (!Dart_IsError(importLibrary) && (tag == kImportTag))
  {
    Dart_Handle result = Dart_LibraryImportLibrary(library, importLibrary);
    if (Dart_IsError(result))
    {
      const char *err = Dart_GetError(result);
      if(err)
      {
        std::cerr << err << std::endl;
      }
    }
  }

  return importLibrary;
}

int main(int a, char* b[])
{
  const char* lib = 
    "#library(\"NativeTest\");\n"
    "class NativeTest extends NativeWrapper {\n"
    "   NativeTest(Dynamic a) { _ctor(a); }"
    "   void _ctor(Dynamic a) native 'Fields_ctor';"
    "   Dynamic crapSalad(Dynamic) native 'Fields_crapSalad';"
    "}\n";

  const char* kScriptChars =
  "#import(\"NativeTest\");"
  "String main() {\n"
  "  NativeTest obj = new NativeTest(45.5);\n"
  "  obj.crapSalad(5.5);"
  "  return obj.crapSalad(5.0).toStringAsFixed(5);"
  "}\n";

  _lookup[std::pair<std::string, uint8_t>("Fields_ctor", 2)] = ctor;
  _lookup[std::pair<std::string, uint8_t>("Fields_crapSalad", 2)] = crapSalad;

  Dart_SetVMFlags(a, (const char**)b);
  Dart_Initialize(isolateCreateCallback, isolateInterruptCallback);

  // Start an Isolate, load a script and create a full snapshot.
  Dart_CreateIsolate(0, 0, 0, 0);
  {
    Dart_EnterScope();  // Start a Dart API scope for invoking API functions.

    Dart_SetLibraryTagHandler(tagHandler);

    _libs["NativeTest"] = Dart_NewString(lib);
    
    // Create a test library and Load up a test script in it.
    Dart_Handle source = Dart_NewString(kScriptChars);
    Dart_Handle url = Dart_NewString("mainSrc");
    Dart_Handle script = Dart_LoadScript(url, source, Dart_Null());
    if(Dart_IsError(script))
    {
      const char *err = Dart_GetError(script);
      if(err)
      {
        std::cerr << err << std::endl;
      }
    }

    Dart_Handle result = Dart_Invoke(script,
      Dart_NewString("main"),
      0,
      0);

    if(Dart_IsError(result))
    {
      const char *err = Dart_GetError(result);
      if(err)
      {
        std::cerr << err << std::endl;
      }
    }

    wchar_t* data = 0;
    if(Dart_IsString16(result))
    {
      intptr_t len = 0;
      Dart_StringLength(result, &len);

      data = new wchar_t[len+1];
      Dart_StringGet16(result, data, &len);
      data[len] = L'\0';
    }

    delete [] data;

    Dart_ExitScope();  // Exit the Dart API scope.
  }
  Dart_ShutdownIsolate();

  return 0;
}
