#ifndef SCPLUGIN_H
#define SCPLUGIN_H

#include "scglobal.h"
#include "aabstractplugin.h"
#include "styperegistry.h"

#include "styperegistry.h"
#include "QSet"

#include "XScriptObject.h"

class ScCoreObject;
class XScriptEngine;
class ScObject;
class ScInputThread;
class ScSurface;
class SDatabaseModel;
class XScriptValue;
class XScriptFunction;

class SCRIPT_EXPORT ScPlugin : public AAbstractPlugin, STypeRegistry::Observer
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

  Q_INVOKABLE bool execute(const QString &filename, const QString &code);

  Q_INVOKABLE bool isDebuggingEnabled();

  XScriptEngine *engine();

  Q_INVOKABLE QWidget *addQMLWindow(const QString &url, const QVariantMap &qmlData);
  Q_INVOKABLE QObject *addQMLSurface(const QString &name, const QString &type, const QString &url, const QVariantMap &qmlData);

  static XScriptValue call(const XScriptFunction &fn, const XScriptObject &th, const XScriptValue *val, xsize argCount);

  void registerScriptGlobal(QObject *);
  void registerScriptGlobal(const QString &, QObject *);
  void registerScriptGlobal(const QString &, const XScriptValue &cl);

signals:
  void debuggingStateChanged(bool enabled);

  void typeAdded(QString type);
  void typeRemoved(QString type);

public slots:
  void enableDebugging(bool enable=true);
  void showDebugger();
  void hideDebugger();

private:
  void initDebugger();
  bool executeFile(const QString &);

  virtual void pluginAdded(const QString &type);
  virtual void pluginRemoved(const QString &type);

  virtual void typeAdded(const SPropertyInformation *);
  virtual void typeRemoved(const SPropertyInformation *);

  XScriptEngine *_engine;

  ScSurface *_surface;

  SDatabaseModel *_model;

  QSet <QString> _includedFiles;
  };

#endif // SCPLUGIN_H
