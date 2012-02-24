#ifndef XSCRIPT_H
#define XSCRIPT_H

class XScript
  {
public:
  XScript(const char *data)
    {
    // Create a string containing the JavaScript source code.
    v8::Handle<v8::String> source = v8::String::New(data);

    // Compile the source code.
    _script = v8::Script::Compile(source);
    }

  void run()
    {
    v8::TryCatch trycatch;
    // Run the script to get the result.
    v8::Handle<v8::Value> result = _script->Run();

    if (result.IsEmpty())
      {
      v8::Handle<v8::Value> exception = trycatch.Exception();
      v8::String::AsciiValue exception_str(exception);
      printf("Exception: %s\n", *exception_str);
      // ...
      }

    // Convert the result to an ASCII string and print it.
    v8::String::AsciiValue ascii(result);
    printf("%s\n", *ascii);
    }

private:
  v8::Handle<v8::Script> _script;
  };


#endif // XSCRIPT_H
