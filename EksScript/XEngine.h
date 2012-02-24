#ifndef XENGINE_H
#define XENGINE_H

class XEngine
  {
public:
  XEngine() : _globalTemplate(v8::ObjectTemplate::New())
  {
  }

private:
  v8::HandleScope _scope;
  v8::Handle<v8::ObjectTemplate> _globalTemplate;
  friend class XContext;
  };

#endif // XENGINE_H
