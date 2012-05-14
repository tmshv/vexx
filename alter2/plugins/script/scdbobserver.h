#ifndef SCDBOBSERVER_H
#define SCDBOBSERVER_H

#include "sobserver.h"
#include "XInterface.h"

class ScDbTreeObserver : public STreeObserver
  {
public:
  ScDbTreeObserver(const XScriptObject &thsObject, const XScriptFunction &function, const XScriptFunction &act);

  void onTreeChange(const SChange *, bool backwards) X_OVERRIDE;
  void actOnChanges() X_OVERRIDE;

  static XInterfaceBase* createInterface();

private:
  XPersistentScriptValue _ths;
  XPersistentScriptValue _fn;
  XPersistentScriptValue _act;
  };

X_SCRIPTABLE_TYPE(ScDbTreeObserver);

#endif // SCDBOBSERVER_H
