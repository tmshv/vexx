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
class ScIO;
class SDatabaseModel;

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

  Q_INVOKABLE void addQMLSurface(const QString &name, const QString &type, const QString &url);

  static QScriptValue call(QScriptValue fn, QScriptValue th, const QList<QScriptValue> &args);

  void registerScriptGlobal(QObject *);
  void registerScriptGlobal(const QString &, QObject *);
  void registerScriptGlobal(const QString &, QScriptClass *cl);
  void registerScriptGlobal(const QString &, const QScriptValue &cl);

signals:
  void debuggingStateChanged(bool enabled);

public slots:
  void enableDebugging(bool enable=true);
  void showDebugger();
  void hideDebugger();

private:
  void initDebugger();
  bool executeFile(const QString &);

  virtual void pluginAdded(const QString &type);
  virtual void pluginRemoved(const QString &type);

  QScriptEngine *_engine;
  QScriptEngineDebugger *_debugger;

  ScSurface *_surface;
  ScEmbeddedTypes *_types;
  ScIO *_io;

  SDatabaseModel *_model;
  };

#endif // SCPLUGIN_H
