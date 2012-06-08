#ifndef SCDBOBSERVER_H
#define SCDBOBSERVER_H

#include "sobserver.h"
#include "XInterface.h"

class ScDbTreeObserver : public STreeObserver
  {
public:
  ScDbTreeObserver(const XScriptObject &thsObject,
                   const XScriptFunction &treeChange,
                   const XScriptFunction &nameChange,
                   const XScriptFunction &act);

  void onTreeChange(const SChange *, bool backwards) X_OVERRIDE;
  void actOnChanges() X_OVERRIDE;

  static XInterfaceBase* createInterface();

private:
  XPersistentScriptValue _ths;
  XPersistentScriptValue _tree;
  XPersistentScriptValue _name;
  XPersistentScriptValue _act;
  };

X_SCRIPTABLE_TYPE_BASE_INHERITED(ScDbTreeObserver, STreeObserver)
namespace XScriptConvert
{
namespace internal
{
template <> struct NativeToJS<ScDbTreeObserver> : public XScript::NativeToJSConvertableTypeInherited<ScDbTreeObserver, STreeObserver> {};
}
}

#endif // SCDBOBSERVER_H
