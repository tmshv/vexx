#include "scplugin.h"
#include "splugin.h"
#include "scsurface.h"
#include "scembeddedtypes.h"
#include "QApplication"
#include "XScriptEngine.h"
#include "XQObjectWrapper.h"
#include "XScriptFunction.h"
#include "UIPlugin.h"
#include "acore.h"
#include "XScriptEngine.h"
#include "scshiftentity.h"
#include "QDir"
#include "aplugin.h"
#include "QDebug"
#include "QMainWindow"
#include "XScriptSource.h"
#include "sdatabasemodel.h"
#include "QGraphicsObject"
#include "qdeclarative.h"
#include "QMLExtensions/scpath.h"
#include "QMLExtensions/scellipse.h"
#include "QMLExtensions/scbrush.h"
#include "QMLExtensions/scnodedisplay.h"
#include "QMLExtensions/scnodeitem.h"
#include "QMLExtensions/scconnectoritem.h"
#include "QMLExtensions/scmousearea.h"
#include "QGLWidget"
#include "scdeclarativesurface.h"
#include "scdbutils.h"
#include "scdbobserver.h"

ALTER_PLUGIN(ScPlugin);

ScPlugin::ScPlugin() : _engine(0), _surface(0), _model(0)
  {
  setObjectName("script");
  }

ScPlugin::~ScPlugin()
  {
  delete _engine;
  delete _model;

  _model = 0;
  _engine = 0;
  _surface = 0;
  }

void ScPlugin::initDebugger()
  {
//  if(_debugger)
//    {
//    return;
//    }

//  _debugger = new QScriptEngineDebugger(this);
//  _debugger->setAutoShowStandardWindow(true);

//  APlugin<UIPlugin> ui(this, "ui");
//  if(ui.isValid())
//    {
//    connect(ui.plugin(), SIGNAL(aboutToClose()), _debugger, SLOT(deleteLater()));
//    }
  }


void ScPlugin::pluginAdded(const QString &type)
  {
  if(type != "db")
    {
    QObject *plug = core()->plugin(type);
    xAssert(plug);
    registerScriptGlobal(type, plug);

    if(type == "ui")
      {
      _surface = new ScSurface(this);

      APlugin<UIPlugin> ui(this, "ui");
      xAssert(ui.isValid());
      ui->addSurface(_surface);

      includePath(":/Sc/StartupUI.js");
      }
    }
  }

void ScPlugin::pluginRemoved(const QString &type)
  {
  _engine->set(type, XScriptValue());

  if(type == "db")
    {
    STypeRegistry::removeTypeObserver(this);
    delete _model;
    _model = 0;
    }
  else if(type == "ui")
    {
    delete _surface;
    _surface = 0;
    }
  }

void ScPlugin::typeAdded(const SPropertyInformation *info)
  {
  _engine->addInterface(info->apiInterface());

  XScriptValue arr = _engine->get("db").get("types");
  XScriptObject obj(arr);
  xAssert(obj.isValid());

  info->apiInterface()->addClassTo(info->typeName(), obj);
  }

void ScPlugin::typeRemoved(const SPropertyInformation *)
  {
  }

XScriptValue printFn(XScriptArguments const &args)
  {
  QString result;
  for (xsize i = 0; i < args.length(); ++i)
    {
    result.append(args.at(i).toString());
    }

  qDebug() << result;

  return XScriptValue();
  }

void ScPlugin::load()
  {
  XProfiler::setStringForContext(ScriptProfileScope, "Script");
  _engine = new XScriptEngine(true);

  _engine->set("print", printFn);

  _engine->addInterface(ScDbTreeObserver::createInterface());

  registerScriptGlobal(this);

  APlugin<SPlugin> db(this, "db");
  xAssert(db.isValid());

  //XScriptObject_engine->set("dbTypes", XScriptValue(XScriptObject::newObject()));

  registerScriptGlobal("db", XScriptConvert::to(&db->db()));

  STypeRegistry::addPropertyGroup(dynamicScriptPropertyGroup());

  STypeRegistry::addTypeObserver(this);
  XScriptObject dbObject = _engine->get("db");
  XScriptObject types = XScriptObject::newObject();
  dbObject.set("types", types);

  types.set("registerType", XScriptValue(XScriptFunction(registerTypeFn)));
  types.set("registerExporter", XScriptValue(XScriptFunction(registerExporterFn)));

  foreach(const SPropertyInformation *t, STypeRegistry::types())
    {
    typeAdded(t);
    }

  qmlRegisterType<ScPath>("VexxQMLExtensions", 1, 0, "Path");
  qmlRegisterType<ScEllipse>("VexxQMLExtensions", 1, 0, "Ellipse");
  qmlRegisterType<ScGradient>("VexxQMLExtensions", 1, 0, "Gradient");
  qmlRegisterType<ScGradientStop>("VexxQMLExtensions", 1, 0, "GradientStop");
  qmlRegisterType<ScPen>("VexxQMLExtensions", 1, 0, "Pen");
  qmlRegisterType<ScNodeDisplay>("VexxQMLExtensions", 1, 0, "NodeDisplay");
  qmlRegisterType<ScPropertyDisplay>("VexxQMLExtensions", 1, 0, "PropertyDisplay");
  qmlRegisterType<ScNodeItem>("VexxQMLExtensions", 1, 0, "NodeItem");
  qmlRegisterType<ScPropertyItem>("VexxQMLExtensions", 1, 0, "PropertyItem");
  qmlRegisterType<ScConnectorItem>("VexxQMLExtensions", 1, 0, "ConnectorItem");
  qmlRegisterType<ScMouseAndScrollArea>("VexxQMLExtensions", 1, 0, "MouseAndScrollArea");
  qmlRegisterUncreatableType<ScMouseEvent>("VexxQMLExtensions", 1, 0, "MouseEventV2", "thing");

#ifdef X_DEBUG
  core()->addDirectory(core()->rootPath() + "../alter2/plugins/script/");
#endif

  xAssert(!_model);
  _model = new SDatabaseModel(&db->db(), &db->db(), SDatabaseModel::NoOptions);

  include("CoreStartup.js");
  }

void ScPlugin::enableDebugging(bool)
  {
//  initDebugger();
//  emit debuggingStateChanged(enable);
//  if(enable)
//    {
//    _debugger->attachTo(_engine);
//    }
//  else
//    {
//    _debugger->detach();
//    }
  }

void ScPlugin::showDebugger()
  {
//  initDebugger();
//  _debugger->standardWindow()->show();
  }

void ScPlugin::hideDebugger()
  {
//  if(_debugger)
//    {
//    _debugger->standardWindow()->hide();
//    }
  }

bool ScPlugin::loadPlugin(const QString &plugin)
  {
  ScProfileFunction
  xAssert(core());
  return core()->load(plugin);
  }

void ScPlugin::includePath(const QString &filename)
  {
  ScProfileFunction

  QFileInfo fInfo(filename);
  QString basePath = fInfo.canonicalFilePath();

  if(!_includedFiles.contains(basePath))
    {
    _includedFiles << basePath;
    bool result = executeFile(filename);
    if(result)
      {
      qDebug() << " Include File" << basePath << " ... Success";
      }
    else
      {
      qWarning() << " Include File" << basePath << " ... Failure";
      }
    }
  else
    {
    qDebug() << " Already Included File" << basePath << " ... Success";
    }
  }

void ScPlugin::include(const QString &filename)
  {
  ScProfileFunction
  qDebug() << "Include File" << filename;
  QString file;
  foreach( const QString &dir, core()->directories() )
    {
    file = dir + QDir::separator() + filename;
    if(QFile::exists(file))
      {
      includePath(file);
      return;
      }
    }
  qWarning() << "Failed to include file " + filename + ", couldn't locate the file in any include directory.";
  }

void ScPlugin::includeFolder(const QString &folder)
  {
  ScProfileFunction
  qDebug() << "Include Folder" << folder;
  QDir dir(folder);
  if(dir.exists())
    {
    foreach(const QFileInfo &filename, dir.entryInfoList(QStringList() << "*.js"))
      {
      includePath(filename.filePath());
      }
    }
  else
    {
    foreach( const QString &dir, core()->directories() )
      {
      QDir relativeDir(dir + QDir::separator() + folder);
      if(relativeDir.exists())
        {
        foreach(const QFileInfo &filename, relativeDir.entryInfoList(QStringList() << "*.js"))
          {
          includePath(filename.filePath());
          }
        }
      }
    }

  qDebug() << "... Success";
  }

void ScPlugin::registerScriptGlobal(QObject *in)
  {
  XScriptValue objectValue = XScriptConvert::to(in);
  _engine->set(in->objectName(), objectValue);
  }

void ScPlugin::registerScriptGlobal(const QString &name, QObject *in)
  {
  XScriptValue objectValue = XScriptConvert::to(in);
  _engine->set(name, objectValue);
  }

void ScPlugin::registerScriptGlobal(const QString &name, const XScriptValue &val)
  {
  _engine->set(name, val);
  }

bool ScPlugin::executeFile(const QString &filename)
  {
  ScProfileFunction
  QFile file(filename);
  if(file.exists() && file.open( QIODevice::ReadOnly))
    {
    QString data(QString::fromUtf8(file.readAll()));
    return execute(filename, data);
    }
  return false;
  }

bool ScPlugin::isDebuggingEnabled()
  {
  return false;//_engine->agent() != 0;
  }

XScriptEngine *ScPlugin::engine()
  {
  return _engine;
  }

XScriptValue ScPlugin::call(const XScriptFunction &fn, const XScriptObject &th, const XScriptValue *val, xsize argCount)
  {
  bool error = false;
  QString message;
  XScriptValue ret = fn.callWithTryCatch(th, argCount, val, &error, &message);

  if(error)
    {
    qCritical() << "Error in script at line " << message << endl;
    return XScriptValue();
    }

  qDebug() << "Script Returned: " << ret.toString() << endl;
  return ret;
  }

QWidget *ScPlugin::addQMLWindow(const QString &url, const QVariantMap &qmlData)
  {
  APlugin<UIPlugin> ui(this, "ui");
  if(!ui.isValid())
    {
    return 0;
    }

  ScDeclarativeWindow *s = new ScDeclarativeWindow(url, _model, qmlData);

  return s;
  }

QObject *ScPlugin::addQMLSurface(const QString &name, const QString &type, const QString &url, const QVariantMap &qmlData)
  {
  APlugin<UIPlugin> ui(this, "ui");
  if(!ui.isValid())
    {
    return 0;
    }

  UISurface::SurfaceType t = (type == "Properties") ? UISurface::PropertiesPage : UISurface::Dock;

  ScDeclarativeSurface *s = new ScDeclarativeSurface(name, url, t, _model, qmlData);
  ui->addSurface(s);

  return s;
  }

bool ScPlugin::execute(const QString &filename, const QString &code)
  {
  ScProfileFunction
  XScriptSource src(filename, code);

  XScriptSource::Error error;
  XScriptValue ret = src.run(&error);

  if(error.hasError())
    {
    qWarning() << "Error " << error.message() << " in script at line " << error.lineNumber();
    qWarning() << error.trace();
    return false;
    }
  else
    {
    XScriptFunction fn(ret);
    if(!fn.isValid())
      {
      qDebug() << "Script Returned: " << ret.toString();
      }
    else
      {
      qDebug() << "Script Returned: [function Function]";
      }
    }
  return true;
  }
