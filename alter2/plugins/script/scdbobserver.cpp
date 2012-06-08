#include "scdbobserver.h"
#include "scplugin.h"
#include "spropertycontainer.h"

ScDbTreeObserver::ScDbTreeObserver(const XScriptObject &thsObject,
                                   const XScriptFunction &tree,
                                   const XScriptFunction &name,
                                   const XScriptFunction &act)
  {
  _ths = XScriptValue(thsObject);
  _tree = XScriptValue(tree);
  _name = XScriptValue(name);
  _act = XScriptValue(act);
  }

void ScDbTreeObserver::onTreeChange(const SChange *c, bool backwards)
  {
  const SPropertyContainer::TreeChange* tree = c->castTo<SPropertyContainer::TreeChange>();
  if(tree)
    {
    XScriptFunction fn(_tree.asValue());
    XScriptObject ths(_ths.asValue());
    xAssert(fn.isValid());

    XScriptValue args[] = {
      XScriptConvert::to(const_cast<SProperty*>(tree->property())),
      XScriptConvert::to(const_cast<SPropertyContainer*>(tree->before(backwards))),
      XScriptConvert::to(const_cast<SPropertyContainer*>(tree->after(backwards))),
      backwards
    };

    ScPlugin::call(fn, ths, args, X_ARRAY_COUNT(args));
    }

  const SProperty::NameChange* name = c->castTo<SProperty::NameChange>();
  if(name)
    {
    XScriptFunction fn(_name.asValue());
    XScriptObject ths(_ths.asValue());
    xAssert(fn.isValid());

    XScriptValue args[] = {
      XScriptConvert::to(const_cast<SProperty*>(name->property())),
      name->before(backwards),
      name->after(backwards),
      backwards
    };

    ScPlugin::call(fn, ths, args, X_ARRAY_COUNT(args));
    }
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
  auto parent = XInterface<STreeObserver>::lookup();
  XInterface<ScDbTreeObserver> *ifc = XInterface<ScDbTreeObserver>::createWithParent("STreeObserver", parent, parent);

  ifc->addConstructor<ScDbTreeObserver *(const XScriptObject &, const XScriptFunction &, const XScriptFunction &, const XScriptFunction &)>();

  ifc->seal();
  return ifc;
  }
