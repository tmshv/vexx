#ifndef SCPLUGIN_H
#define SCPLUGIN_H

#include "scglobal.h"
#include "aabstractplugin.h"

class ScCoreObject;
class QScriptEngine;
class QScriptValue;
class QScriptClass;
class QScriptEngineDebugger;
class ScObject;
class ScInputThread;
class ScSurface;
class ScEmbeddedTypes;

class SCRIPT_EXPORT ScPlugin : public AAbstractPlugin
  {
  Q_OBJECT
public:
  ScPlugin();
  ~ScPlugin();

  void load();

  // script accessor for loading a plugin
  Q_INVOKABLE bool loadPlugin(const QString &plugin);

  // include a path explicitly ../scripts/script.js
  Q_INVOKABLE void includePath(const QString &file);
  // include "../scripts" and execute all the js files insisde
  Q_INVOKABLE void includeFolder(const QString &folder);
  // find "script.js" somewhere in the apps loaded plugin directories
  Q_INVOKABLE void include(const QString &folder);

  Q_INVOKABLE bool execute(const QString &);

  Q_INVOKABLE bool isDebuggingEnabled();

  QScriptEngine *engine();

  void registerScriptGlobal(QObject *);
  void registerScriptGlobal(const QString &, QScriptClass *cl);
  void registerScriptGlobal(const QString &, const QScriptValue &cl);

signals:
  void debuggingStateChanged(bool enabled);

public slots:
  void enableDebugging(bool enable=true);
  void showDebugger();
  void hideDebugger();

private:
  bool executeFile(const QString &);

  QScriptEngine *_engine;
  QScriptEngineDebugger *_debugger;

  ScSurface *_surface;
  ScEmbeddedTypes *_types;
  };

#endif // SCPLUGIN_H
