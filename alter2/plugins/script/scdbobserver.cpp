#include "scdbobserver.h"
#include "scplugin.h"

ScDbTreeObserver::ScDbTreeObserver(const XScriptObject &thsObject, const XScriptFunction &fn, const XScriptFunction &act)
  {
  _ths = XScriptValue(thsObject);
  _fn = XScriptValue(fn);
  _act = XScriptValue(act);
  }

void ScDbTreeObserver::onTreeChange(const SChange *, bool backwards)
  {
  XScriptFunction fn(_fn.asValue());
  XScriptObject ths(_ths.asValue());
  xAssert(fn.isValid());

  ScPlugin::call(fn, ths, 0, 0);
  }

void ScDbTreeObserver::actOnChanges()
  {
  XScriptFunction fn(_act.asValue());
  XScriptObject ths(_ths.asValue());
  xAssert(fn.isValid());

  ScPlugin::call(fn, ths, 0, 0);
  }

XInterfaceBase* ScDbTreeObserver::createInterface()
  {
  XInterface<ScDbTreeObserver> *ifc = XInterface<ScDbTreeObserver>::create("STreeObserver");

  return ifc;
  }
