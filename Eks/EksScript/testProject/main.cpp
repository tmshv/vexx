#define _ITERATOR_DEBUG_LEVEL 0

#include "XScriptEngine.h"
#include "XInterface.h"
#include "XAssert"

#include "XScriptValueDartInternals.h"
#include "dart_api.h"
#include "DartUtils.h"
#include <map>
#include <string>
#include <iostream>

struct Nummer
{
  double num;

  Nummer(float n) : num(n) {}

  QString crapSalad(float newNum)
  {
    num += newNum;

    return QString::number(num);
  }
};

X_SCRIPTABLE_TYPE(Nummer)

int main(int a, char* b[])
{
  XScriptEngine::initiate();

  XScriptEngine eng(false);

  XInterface<Nummer>* nummer = XInterface<Nummer>::create("Nummer");
  nummer->addConstructor<Nummer* (float)>();
  nummer->addMethod<QString(float), &Nummer::crapSalad>("crapSalad");
  
  const char* kScriptChars =
  "#import(\"NativeTest\");"
  "String main() {\n"
  "  Nummer obj = new Nummer(45.5);\n"
  "  obj.crapSalad(5.5);"
  "  return obj.crapSalad(5.0).toStringAsFixed(5);"
  "}\n";

  {
    Dart_EnterScope();  // Start a Dart API scope for invoking API functions.

    
    // Create a test library and Load up a test script in it.
    Dart_Handle source = Dart_NewString(kScriptChars);
    Dart_Handle url = Dart_NewString("mainSrc");
    Dart_Handle script = Dart_LoadScript(url, source, Dart_Null());
    CHECK_HANDLE(script)

    Dart_Handle result = Dart_Invoke(script,
      Dart_NewString("main"),
      0,
      0);
    CHECK_HANDLE(result)

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

  return 0;
}
