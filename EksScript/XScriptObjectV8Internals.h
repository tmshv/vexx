#ifndef XSCRIPTOBJECTV8INTERNALS_H
#define XSCRIPTOBJECTV8INTERNALS_H

#include "v8.h"

class XScriptObject;
class XInterfaceObject;
class XScriptFunction;

XScriptObject fromHandle(v8::Handle<v8::Value>);
XInterfaceObject fromObjectHandle(v8::Handle<v8::Object>);
XScriptFunction fromFunction(v8::Handle<v8::Function>);

v8::Handle<v8::Value> getV8Internal(const XScriptObject&);
v8::Handle<v8::Object> getV8Internal(const XInterfaceObject &o);

v8::Handle<v8::Value> *getV8Internal(const XScriptObject*);

#endif // XSCRIPTOBJECTV8INTERNALS_H
