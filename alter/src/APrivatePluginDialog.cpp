#include "APrivatePluginDialog.h"
#include "ui_APrivatePluginDialog.h"
#include "APrivateWindow.h"
#include "QLabel"
#include "QComboBox"
#include "QDir"
#include "QLibrary"
#include "QSettings"
#include "../SDK/ALog"
#include "../SDK/APlugin"

#include "QDebug"

#ifdef Q_WS_MAC
#include "CoreFoundation/CFBundle.h"
#endif

/** \cond INTERNAL
  */

// copied and adapted from the qt documentation, basically it retrieves the direcory of the *.app
// directory on a mac, or the directory of the executable on everything else.
QString rootPath()
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

/** \class APrivatePluginDialog
  \brief Handles loading and unloading of plugins.
  \todo This should be integrated with workspaces, so you can load a workspace which has different plugins!
  \todo Separate the Gui and the plugin loading
  */

/** Create a plugin dialog.
  */
APrivatePluginDialog::APrivatePluginDialog( APrivateWindow *parent, bool userMode ) : m_ui( new Ui::pluginDialog ), _window( parent )
    {
    _userMode = userMode;
    // setup ui
    m_ui->setupUi(this);

    // this should have a Gui, but for now it loads plugins stored in app()/plugins/
    addPluginDirectory( rootPath() + "/plugins/" );

    // setup tp clean state
    _pluginsScroller = 0;
    clearPlugins( );
    _currentLoading = 0;
    }

APrivatePluginDialog::~APrivatePluginDialog()
    {
    // delete ui first
    delete m_ui;

    if( _userMode )
        {
        // store plugin state to disk
        QSettings settings;
        foreach( QString key, _currentList.keys() )
            {
            if( _currentList.value( key )->currentIndex() == 2 )
                {
                settings.remove( "Libraries/" + key );
                }
            }
        }

    // delete plugins
    foreach( QString key, _loadedLibrary.keys() )
        {
        deinitPlugin( _loadedLibrary.value( key ).lib );
        delete _loadedLibrary.value( key ).lib;
        }
    }

/** Adds a directory to the list searched for plugins
  */
void APrivatePluginDialog::addPluginDirectory( const QString &in )
    {
    _directories.push_back( in );
    }

/** Request a plugin called \p in is loaded ( only works if it isnt set to "Don't Load"
  */
bool APrivatePluginDialog::request( QString in )
    {
    // for each plugin directory
    foreach( QString d, _directories )
        {
        // find the files in the list
        QFileInfoList files = QDir( d ).entryInfoList();
        foreach( QFileInfo f, files )
            {
            // check if the filename is a valid one for library "in"
            if( in == f.baseName( ) || ( "lib"+in) == f.baseName( ) )
                {
                // try to load, make sure we take account of recursive loading

                // store the currently loading library
                loadedLibrary *cLBac( _currentLoading );
                // try to load a library ( possibly recursing )
                bool t = tryLoad( f, TRUE );
                // reset the currently loading
                _currentLoading = cLBac;
                // if didnt manage to load
                if( !t )
                    {
                    ALog << "The plugin '" << in << "' couldn't be loaded" << endl;
                    }
                // return status
                return t;
                }
            }
        }
    ALog << "Couldn't find plugins '" << in << "' for loading" << endl;
    return FALSE;
    }

/** Return the core of the plugin "in"
  \deprecated
  */
void *APrivatePluginDialog::access( QString in )
    {
    // try to find the library
    QHash <QString, loadedLibrary>::iterator it = _loadedLibrary.find( "lib"+in );
    // if we couldnt find the library "libin" ( linux ), try to find in "in" ( windows / mac )
    if( it == _loadedLibrary.end() )
        {
        it = _loadedLibrary.find( in );
        }

    // if we found something
    if( it != _loadedLibrary.end() )
        {
        // and it has a core
        if( it->core == 0 )
            {
            AWarning << "Cross Plugin Access|Trying to access a plugin with no specified core" << endl;
            }
        // return core
        return it->core;
        }
    AWarning << "Cross Plugin Access|Couldnt find the plugin " << in << endl;
    return 0;
    }

/** Called on show event for dialog.
  */
void APrivatePluginDialog::showEvent( QShowEvent * )
    {
    refreshDialog();
    }

/** Refresh the dialog from hard disk.
  */
void APrivatePluginDialog::refreshDialog()
    {
    QHash <QString, bool> inList;
    // clear dialog
    clearPlugins( );

    QSettings settings;
    // for each plugin direcotry
    foreach( QString d, _directories )
        {
        QFileInfoList files = QDir( d ).entryInfoList();
        if( files.size() )
            {
            // if there are files in the directory, then add it to the Gui
            addDirectory( d );
            }
        // for each file in the list
        foreach( QFileInfo f, files )
            {
            //if the file is a library
            if( QLibrary::isLibrary( f.canonicalFilePath( ) ) && inList.find( f.baseName() ) == inList.end() )
                {
                // add the plugin with the state in the user settings
                inList[f.baseName()] = TRUE;
                addPlugin( f.baseName(), (pluginMode)settings.value( "Libraries/" + f.baseName(), 0 ).toInt() );
                }
            }
        }
    addSpacer( );
    }

/** Clear the plugins from the Gui
  */
void APrivatePluginDialog::clearPlugins( )
    {
    // delete the scroll area
    if( _pluginsScroller )
        {
        _pluginsScroller->deleteLater();
        _pluginsScroller = 0;
        }

    // re allocate the scroll area, and set it up
    _pluginsScroller = new QScrollArea( this );
    _pluginsScroller->setAutoFillBackground(false);
    _pluginsScroller->setFrameShadow(QFrame::Sunken);
    _pluginsScroller->setMidLineWidth(0);
    _pluginsScroller->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _pluginsScroller->setWidgetResizable(true);
    // allocate the scrolled widget, and set it up
    _pluginsContent = new QWidget();
    _pluginsContent->setObjectName( "bgWidget" );
    _pluginsContent->setGeometry(QRect(0, 0, 335, 292));
    _pluginsContentLayout = new QVBoxLayout( _pluginsContent );
    _pluginsScroller->setWidget( _pluginsContent );

    // add a layout.
    m_ui->verticalLayout->insertWidget( 0, _pluginsScroller );

    // clear pointer list
    _currentList.clear();
    }

/** Add a spacer to the Gui plugin layout
  */
void APrivatePluginDialog::addSpacer( )
    {
    QSpacerItem *Spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    _pluginsContentLayout->addItem(Spacer);
    }

/** Add a directory called \p dirName to the Gui plugin layout
  */
void APrivatePluginDialog::addDirectory( QString dirName )
    {
    // create a label and set it up
    QLabel *pluginLabel = new QLabel( _pluginsContent );
    QFont font;
    font.setBold(true);
    font.setWeight(75);
    pluginLabel->setFont(font);
    pluginLabel->setText( dirName );

    // add it to the layout
    _pluginsContentLayout->addWidget( pluginLabel );
    }

/** Add a plugin called \p pluginName to the Gui layout. The plugin is entered with the level \p index
  */
void APrivatePluginDialog::addPlugin( QString pluginName, pluginMode index )
    {
    // a layout
    QHBoxLayout *pluginSection = new QHBoxLayout();
    QLabel *pluginLabel = new QLabel(_pluginsContent);
    pluginLabel->setText( pluginName );

    // add the label
    pluginSection->addWidget( pluginLabel );

    // create a combobox, with specified levels
    QComboBox *pluginChoice = new QComboBox(_pluginsContent);
    pluginChoice->insertItems( 0, QStringList() << tr( "Don't load" )
                                                << tr( "Load on demand" )
                                                << tr( "Load for this session" )
                                                << tr( "Always load" ) );
    // set current level
    pluginChoice->setCurrentIndex( index );

    // add the plugin to the list
    _currentList[pluginName] = pluginChoice;

    // connect the changed signal to this
    connect( pluginChoice, SIGNAL(activated(int)), this, SLOT(refreshPluginModes()) );

    // add the widget to the layoiut
    pluginSection->addWidget(pluginChoice);

    // add the new layout to the plugin layout
    _pluginsContentLayout->addLayout(pluginSection);
    }

/** Set the core of the currently loading plugin
  */
void APrivatePluginDialog::setCurrentCore( void *in )
    {
    if( _currentLoading )
        {
        _currentLoading->core = in;
        }
    }

/** Called to load the plugins specified by the UI
  */
void APrivatePluginDialog::refreshPluginModes()
    {
    // for each loaded plugin
    QSettings settings;
    foreach( QString key, _currentList.keys() )
        {
        // find the Gui value
        QComboBox *value( _currentList.value( key ) );
        // and set the user settings value.
        if( value->currentIndex() != 0 )
            {
            settings.setValue( "Libraries/" + key, value->currentIndex() );
            }
        else if( settings.value( "Libraries/" + key ) != value->currentIndex() )
            {
            settings.remove( "Libraries/" + key );
            }
        }
    // ensure the plugins specified are loaded
    ensureLoaded();
    // refresh the Gui from the specified use values
    refreshDialog();
    }

/** Try to load the file \p f, \p requested holds whether a plugin requested it. Return true if loaded successfully.
  */
bool APrivatePluginDialog::tryLoad( QFileInfo f, bool requested )
    {
    int required = 2;
    // set the level we can load up to
    if( requested )
        {
        required = 1;
        }

    // find the level of the requested plugin
    QSettings settings;
    int level( settings.value( "Libraries/" + f.baseName( ) ).toInt( ) );

    // if we can load the plugin
    if( ( !_userMode && requested ) || ( _userMode && level >= required ) )
        {
        // test the file is a library
        if( QLibrary::isLibrary( f.canonicalFilePath( ) ) &&
            _loadedLibrary.find( f.baseName( ) ) == _loadedLibrary.end( ) )
            {
            // setup the library structure
            loadedLibrary &lib = _loadedLibrary[f.baseName( )];
            lib.lib = new QLibrary( f.canonicalFilePath(), this );
            lib.lib->setLoadHints( QLibrary::ExportExternalSymbolsHint | QLibrary::ResolveAllSymbolsHint );
            lib.core = 0;
            lib.level = level;

            // if Qt already loaded the library and theres an error string
            if( lib.lib->isLoaded() && lib.lib->errorString() != "" )
                {
                // error message from the library loading
                ALog << lib.lib->errorString() << endl;
                _loadedLibrary.remove( f.baseName( ) );
                _currentLoading = 0;
                }
            else
                {
                // try to resolve the symbols
                if( !((initialiseFn)_loadedLibrary[f.baseName( )].lib->resolve( UNINIT_FN_NAME )) ||
                        !((deinitialiseFn)_loadedLibrary[f.baseName( )].lib->resolve( INIT_FN_NAME )) )
                    {
                    // print error messages
                    ALog<< "The plugin '" <<f.fileName() << "' is not a valid " << _window->document().programName() << " plugin:" << endl
                        << _loadedLibrary[f.baseName( )].lib->errorString() << endl;
                    _loadedLibrary.remove( f.baseName( ) );
                    _currentLoading = 0;
                    }
                else
                    {
                    // try to init the plugin
                    _currentLoading = &lib;
                    return initPlugin( _currentLoading->lib );
                    _currentLoading = 0;
                    }
                }
            }
        else if( _loadedLibrary.find( f.baseName( ) ) != _loadedLibrary.end( ) )
            {
            // we already loaded it
            return TRUE;
            }
        }
    else if( settings.value( "Libraries/" + f.baseName( ) ).toInt( ) != 1 )
        {
        // we shouldnt have the plugin loaded, make sure it isnt
        if( QLibrary::isLibrary( f.canonicalFilePath( ) ) &&
            _loadedLibrary.find( f.baseName( ) ) != _loadedLibrary.end( ) )
            {
            // its loaded, so unload it.
            deinitPlugin( _loadedLibrary[f.baseName( )].lib );
            delete _loadedLibrary[f.baseName( )].lib;
            _loadedLibrary.remove( f.baseName( ) );
            }
        }
    return FALSE;
    }

/** Ensures all the plugins in the list are loaded
  */
void APrivatePluginDialog::ensureLoaded( )
    {
    // set to make sure we dont try to load the same plugin twice
    QSet <QString> tried;

    // for eahc directory
    foreach( QString d, _directories )
        {
        // foreach file in list
        QFileInfoList files = QDir( d ).entryInfoList();
        foreach( QFileInfo f, files )
            {
            // if we havent laready got it loaded ( unix has version number as file extensions )
            if( !tried.contains( f.baseName() ) )
                {
                // try to laod the plugin
                tryLoad( f );
                tried.insert( f.baseName() );
                }
            }
        }
    }

/** Initiate the plugin in \p lib.
  */
bool APrivatePluginDialog::initPlugin( QLibrary *lib )
    {
    // try to resolve the initiate function
    xAssert( lib->resolve( INIT_FN_NAME ) );

    // grab the function into a pointer
    initialiseFn fn = (initialiseFn) lib->resolve( INIT_FN_NAME );

    ALog << "Load plugin " << QFileInfo(lib->fileName()).baseName() << "... " << endl;

    // find the plugin response ( call the dlls function )
    int resp = (*fn)( _window->sdk() );

    // print out a status message
    if( resp == APlugin::Success )
        {
        ALog << "\t... " << QFileInfo(lib->fileName()).baseName() << " complete" << endl;
        return TRUE;
        }
    else
        {
        ALog << "\t... " << QFileInfo(lib->fileName()).baseName() << " complete" << endl;
        }
    return FALSE;
    }

/** Uninitate the function in \p lib
  */
void APrivatePluginDialog::deinitPlugin( QLibrary *lib )
    {
    // make sure we can find the un initate function
    xAssert( lib->resolve( UNINIT_FN_NAME ) );
    // resolve the function pointer
    initialiseFn fn = (initialiseFn) lib->resolve( UNINIT_FN_NAME );
    // print a message
    ALog<<"Unload plugin "<<QFileInfo(lib->fileName()).baseName()<<"...";
    // call the function
    (*fn)( _window->sdk() );
    ALog<<"... "<<QFileInfo(lib->fileName()).baseName() << " complete"<<endl;
    }

/** Save the plugins state to a byte array.
  \todo Use and complete this function.
  */
QByteArray APrivatePluginDialog::saveState()
    {
    QByteArray ret;
    QDataStream stream( &ret, QIODevice::WriteOnly );

    // create a list of data
    QMap <QString, unsigned int> list;
    foreach( QString key, _loadedLibrary.keys() )
        {
        list.insert( key, _loadedLibrary.value( key ).level );
        }

    // stream plugin data to ret.
    stream << list;
    return ret;
    }

/** Restore the plugin state from a byte array \p in.
  */
void APrivatePluginDialog::restoreState( const QByteArray &in )
    {
    // copy source array
    QByteArray arr( in );
    QDataStream stream( &arr, QIODevice::ReadOnly );

    // read list
    QMap <QString, unsigned int> list;
    stream >> list;
    foreach( QString key, list.keys() )
        {
        //qDebug() << l.key() << l.value();
        }
    }

/** \endcond
  */
