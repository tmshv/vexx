#include "acore.h"
#include "QApplication"
#include "QStringList"
#include "aabstractplugin.h"
#include "QFile"
#include "QDomDocument"
#include "QDebug"
#include "QLibrary"
#include "QDir"

#ifdef Q_WS_MAC
#include "CoreFoundation/CFBundle.h"
#endif

QString ACore::rootPath()
    {
#ifdef Q_WS_MAC
    CFURLRef appUrlRef = CFBundleCopyBundleURL( CFBundleGetMainBundle( ) );
    CFStringRef macPath = CFURLCopyFileSystemPath( appUrlRef, kCFURLPOSIXPathStyle );
    const char *pathPtr = CFStringGetCStringPtr( macPath, CFStringGetSystemEncoding( ) );
    QString ret( pathPtr );
    CFRelease( appUrlRef );
    CFRelease( macPath );
    // apparently this doesnt work on japanese language machines... so just in case...
    if( ret != "" )
        {
        QDir dir( QFileInfo(ret).absoluteDir() );
        return dir.absolutePath();
        }
    else
        {
        // couldnt find the plugin
        return "/";
        }
#else
    return QCoreApplication::applicationDirPath();
#endif
    }

class ACorePrivate
    {
public:
    ACorePrivate( int argv, char **argc ) : app( argv, argc ) { }

    struct Plugin
        {
        AAbstractPlugin *plugin;
        QString filename;
        QString name;
        QString directory;
        };

    QApplication app;
    XList <Plugin> installed;
    XList <QString> directories;
    };

ACore::ACore( int argv, char **argc ) : _priv(new ACorePrivate(argv, argc))
    {
    _priv->directories << rootPath() + "/plugins/";
    }

ACore::~ACore()
  {
  foreach(QObject *child, children())
  {
    AAbstractPlugin *plugin = qobject_cast<AAbstractPlugin*>(child);
    if(plugin)
      {
      plugin->unload();
      plugin->setCore( 0 );
      }
    delete child;
  }
  delete _priv;
  }

void ACore::quit()
  {
  QApplication::quit();
  }

int ACore::execute()
  {
  return _priv->app.exec();
  }

bool ACore::isLoaded( const QString &name ) const
  {
  foreach( const ACorePrivate::Plugin &plugin, _priv->installed )
    {
    if( plugin.name == name )
      {
      return true;
      }
    }
  return false;
  }

bool ACore::load( const QString &name )
  {
  if(isLoaded(name))
    {
    return true;
    }

  foreach( QString dirName, _priv->directories )
    {
    QFile file( dirName + QDir::separator() + name + ".aml" );
    if( file.exists() && file.open( QIODevice::ReadOnly ) )
      {
      QDomDocument doc;
      doc.setContent( QString::fromUtf8( file.readAll() ) );

      QDomElement docElem = doc.documentElement();

      QDomElement requires( docElem.firstChildElement("requires") );
      while( !requires.isNull() )
        {
        if( requires.attribute("type") == "library" )
          {
          QString requiresName( requires.firstChildElement("name").text() );
          if( !load( requiresName ) )
            {
            qDebug() << "Dependancies for" << name << "not met. Couldn't find plugin" << requiresName << ".";
            return false;
            }
          }

        requires = requires.nextSiblingElement( "requires" );
        }

      ACorePrivate::Plugin plugin;

      plugin.directory = dirName;
      plugin.name = name.toLower();
      plugin.filename = docElem.firstChildElement( "filename" ).text();

      typedef AAbstractPlugin *(*InitPrototype)( );
      InitPrototype initFunction = (InitPrototype)QLibrary::resolve( dirName + QDir::separator() + plugin.filename, "initAlterPlugin" );
      if( initFunction )
        {
        plugin.plugin = initFunction( );

        plugin.plugin->setCore( this );
        plugin.plugin->setObjectName(plugin.name);
        plugin.plugin->load();

        plugin.plugin->setParent(this);

        // inform new plugin of already installed types
        for( int x=0; x<_priv->installed.size(); x++ )
          {
          plugin.plugin->pluginAdded(_priv->installed[x].plugin->objectName());
          }

        _priv->installed << plugin;

        // install old plugins of new type
        for( int x=0; x<_priv->installed.size(); x++ )
          {
          if(_priv->installed[x].plugin != plugin.plugin)
            {
            _priv->installed[x].plugin->pluginAdded(plugin.plugin->objectName());
            }
          }
        }
      else
        {
        qDebug() << "Couldn't load '" << dirName + QDir::separator() + plugin.filename << "', it isn't a plugin.";
        return false;
        }

      return true;
      }
    }
  qDebug() << "Couldn't find the plugin " << name << ".";
  return false;
  }

void ACore::unload( const QString &in )
  {
  for( int x=0; x<_priv->installed.size(); x++ )
    {
    ACorePrivate::Plugin& plugin = _priv->installed[x];
    if( plugin.name == in )
      {
      _priv->installed.removeAt( x );

      for( int x=0; x<_priv->installed.size(); x++ )
        {
        if(_priv->installed[x].plugin != plugin.plugin)
          {
          _priv->installed[x].plugin->pluginRemoved(plugin.plugin->objectName());
          }
        }

      plugin.plugin->unload();
      plugin.plugin->setCore( 0 );
      }
    }
  }

QStringList ACore::loadable() const
  {
  QStringList ret;
  foreach( QString dirName, _priv->directories )
    {
    QDir dir( dirName );
    foreach( QFileInfo info, dir.entryInfoList( QStringList() << "*.aml" ) )
      {
      ret << info.baseName();
      }
    }

  return ret;
  }

QStringList ACore::directories() const
  {
  return _priv->directories;
  }

void ACore::addDirectory(const QString &in)
  {
  _priv->directories << in;
  }

void ACore::removeDirectory(const QString &in)
  {
  _priv->directories.removeAll( in );
  }

bool ACore::isDebugUIEnabled() const
  {
#ifdef X_DEBUG
  return true;
#else
  return false;
#endif
  }

QObject *ACore::plugin(const QString &pluginName)
  {
  foreach( ACorePrivate::Plugin plugin, _priv->installed )
    {
    if(plugin.name == pluginName)
      {
      return plugin.plugin;
      }
    }
  qDebug() << "Failed to find plugin" << pluginName;
  return 0;
  }
