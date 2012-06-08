#include "scconsole.h"

XInterfaceBase* ScConsole::createInterface()
  {
  auto ifc = XInterface<ScConsole>::create("console");

  ifc->addFunction("log", 0, 1, logJS, logDart);

  ifc->seal();
  return ifc;
  }

XScriptValue ScConsole::logJS(XScriptArguments const &args)
  {
  QString result;
  for (xsize i = 0; i < args.length(); ++i)
    {
    result.append(args.at(i).toString());
    }

  qDebug() << result;

  return XScriptValue();
  }

void ScConsole::logDart(XScriptDartArguments argv)
  {
  XScriptDartArgumentsNoThis args(argv);
  QString result;
  for (xsize i = 0; i < args.length(); ++i)
    {
    result.append(args.at(i).toString());
    }

  qDebug() << result;
  }
