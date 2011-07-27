#include "APrivateWindow.h"
#include "QSettings"
#include "../SDK/AFileDialog"
#include "QMessageBox"
#include "QToolBar"
#include "QDockWidget"
#include "QMenu"
#include "ui_APrivateWindow.h"
#include "../SDK/AAbstractDock"
#include "APrivateLogDock.h"
#include "../SDK/AAbstractToolBar"
#include "QDebug"
#include "APrivateCustomDock.h"
#include "QClipboard"
#include "QCloseEvent"
#include "APrivateSettingsEntity.h"

#define RECENT_FILE_LIST_SIZE 5

/** \cond INTERNAL
  */

/** \class APrivateWindowWarningHandler
  \brief APrivateWindowWarningHandler is a custom warning handler that only accepts warnings of type APPWARNING_ID
  */

/**
  \class APrivateWindow
  \brief APrivateWindow is the main window class, it holds all data relevant to the application and manages all application modules.
  */

/** Create an APrivateWindow object
  \param appName The name of the application
  \param orgName The organisation name
  \param domainName The domain of the organisation
  \param version The version of the application
  \param parent The parent of the main window ( usually 0 )
  */
APrivateWindow::APrivateWindow(QIcon icon, QString appName, bool userPluginInterface, bool needsCreateMenu, QString orgName, QString domainName, QString version, QWidget *parent )
        : QMainWindow( parent ), _document( appName, orgName, domainName, version ),
        _plugins( new APrivatePluginDialog( this, userPluginInterface ) ), ui(new Ui::appBase), _log( 0 ),
        _centralDock( 0 ), _currentContextDock( 0 ), _sdk( this )
    {
    // setup global application data
    QCoreApplication::setApplicationName( appName );
    QCoreApplication::setOrganizationName( orgName );
    QCoreApplication::setOrganizationDomain( domainName );
    QCoreApplication::setApplicationVersion( version );

    // icon
    QApplication::setWindowIcon( icon );

    // setup form based UI
    ui->setupUi(this);

    if( !needsCreateMenu )
        {
        delete ui->menuCreate;
        ui->menuCreate = 0;
        }

    // setup the workspace manager
    _workspace = new APrivateWorkspaceDialog( this );

    // we like dock nesting
    setDockNestingEnabled( TRUE );

    // our basic log
    _log = new APrivateLogDock;
    addDockedItem( "Log", _log );

    // setup plugins
    _plugins->ensureLoaded();

    // developing code
#if 0
    QByteArray state( _plugins->saveState() );
    _plugins->restoreState( state );
#endif


    // connect UI to this class
    connect( ui->actionNew, SIGNAL( triggered() ), this, SLOT( fileNew() ) );
    connect( ui->actionOpen, SIGNAL( triggered() ), this, SLOT( fileOpen() ) );
    connect( ui->actionSave, SIGNAL( triggered() ), this, SLOT( fileSave() ) );
    connect( ui->actionSave_As, SIGNAL( triggered() ), this, SLOT( fileSaveAs() ) );
    connect( ui->actionExit, SIGNAL( triggered() ), this, SLOT( close() ) );
    connect( ui->menuFile, SIGNAL( aboutToShow() ), this, SLOT( rebuildFileMenu() ) );
    connect( ui->menuWindow, SIGNAL( aboutToShow() ), this, SLOT( rebuildWindowMenu() ) );
    connect( ui->menuWindow, SIGNAL( triggered( QAction * ) ), this, SLOT( pollWindowMenuStates( QAction * ) ) );
    connect( ui->menuRecent_Files, SIGNAL(triggered(QAction*)), this, SLOT(openRecent(QAction*)) );

    if( ui->menuCreate )
        {
        connect( ui->menuCreate, SIGNAL( triggered( QAction * ) ), this, SLOT( createTriggered( QAction * ) ) );
        connect( ui->menuCreate, SIGNAL( aboutToShow() ), this, SLOT( rebuildCreateMenu() ) );
        }

    // We handle Warning messages
    xRegisterMessageHandler( _messageHandler );
    connect( &_messageHandler, SIGNAL(handle(XString)), this, SLOT(handle(XString)) );

    // Setup the window title
    refreshWindowTitle();

    // add undo actions to the edit toolbar
    QAction *undo = document().undoStack().createUndoAction( ui->menuEdit, "Undo" );
    QAction *redo = document().undoStack().createRedoAction( ui->menuEdit, "Redo" );
    undo->setShortcut( QKeySequence::Undo );
    redo->setShortcut( QKeySequence::Redo );
    ui->menuEdit->addAction( undo );
    ui->menuEdit->addAction( redo );
    ui->menuEdit->addSeparator();

    // Add clipboard functions to the edit toolbar
    QAction *cut( ui->menuEdit->addAction( "Cut", this, SLOT(cut()) ) );
    cut->setShortcut( QKeySequence::Cut );
    QAction *copy( ui->menuEdit->addAction( "Copy", this, SLOT(copy()) ) );
    copy->setShortcut( QKeySequence::Copy );
    QAction *paste( ui->menuEdit->addAction( "Paste", this, SLOT(paste()) ) );
    paste->setShortcut( QKeySequence::Paste );
    ui->menuEdit->addSeparator();

    // delete selection
    QAction *del( ui->menuEdit->addAction( "Delete Selection", this, SLOT(deleteSelection()) ) );
    del->setShortcut( QKeySequence::Delete );
    ui->menuEdit->addSeparator();

    if( userPluginInterface )
        {
        ui->menuEdit->addAction( "Plugins", _plugins, SLOT( show( ) ) );
        }

    // install event filter for grouping commands
    QCoreApplication::instance()->installEventFilter( this );

    // modified indicator
    connect( &document(), SIGNAL(modified(bool)), this, SLOT(modifiedChanged(bool)) );
    document().newFile();

    // add the creator for a settings entity, so we can load them from file
    _sdk.addCreator( APrivateSettingsEntity::identifier(), APrivateSettingsEntity::create, FALSE );

    ALog<<"Load Complete"<<endl;
    }

/** Gets the plugin manager for this class
  */
APrivatePluginDialog *APrivateWindow::pluginManager()
    {
    return _plugins;
    }

/** Destroy the app
  */
APrivateWindow::~APrivateWindow()
    {
    delete _workspace;
    delete _plugins;

    delete ui;

    // delete log last
    if( _log )
        {
        delete _log;
        _log = 0;
        }
    }

/** Restore the state of the application saved on exit. Call after plugins are initiated
  */
void APrivateWindow::restoreState( QString ctrl, QByteArray recommended )
    {
    _workspace->initiate( recommended );

    // settings can grab application persistant settings
    QSettings settings;

    QString central = settings.value( "centralDock", ctrl ).toString();
    // if we had a central dock last close
    if( central != "" )
        {
        // try to re activate it now
        foreach( QString key, _dockedWidgets.keys() )
            {
            if( key == central )
                {
                setCentralDock( _dockedWidgets.value( key ) );
                break;
                }
            }
        }

    // also restore windoe state we had last time.
    setGeometry( settings.value( "windowGeometry", QRect( 50, 50, 700, 500 ) ).toRect() );
    setWindowState( Qt::WindowStates( settings.value( "windowState" ).toInt() ) );
    }

/** Add a docked widget \p dockWidget called \p str to the application, this may not be a QDockWidget
  \sa removeDockedItem()
  */
void APrivateWindow::addDockedItem( QString str, AAbstractDock *dockWidget )
    {
    // set the qobjects name for use when saved
    dockWidget->setName( str );

    // create a dock widget wrapper for the class
    APrivateCustomDockWidget *dock( new APrivateCustomDockWidget( dockWidget, this ) );
    // add the wrapper
    addDockWidget( Qt::LeftDockWidgetArea, dock );
    // store the variables
    _dockedWidgets[str] = dockWidget;
    _qDocks[str] = dock;

    // tell the dock its been added
    dockWidget->setApp( &_sdk );
    dockWidget->added();
    }

/**Remove a docked widget called \p in from the application
  \sa addDockedItem()
  */
void APrivateWindow::removeDockedItem( QString in )
    {
    // check it exists
    if( _dockedWidgets.contains( in ) )
        {
        // stop it beign the central widget
        if( _dockedWidgets.find( in ).value()->base() == centralWidget() )
            {
            setCentralWidget( 0 );
            }
        // delete it
        delete _dockedWidgets[in];
        _dockedWidgets.remove( in );
        // and its dock
        delete _qDocks[in];
        _qDocks.remove( in );
        }
    }

/** Add a toolbar \p in called \p name to the application.
  \sa removeToolBar()
  */
void APrivateWindow::addToolBar( QString name, AAbstractToolBar *in )
    {
    // add it to our array
    _toolBars.insert( name, in );
    // set up object names
    in->setWindowTitle( name );
    in->setObjectName( name );
    // insert the toolbar to the mainwindow
    QMainWindow::addToolBar( in );

    // tell it its been added
    in->setApp( &_sdk );
    in->added();
    }

/** Add a toolbar called \p in to the application.
  \sa addToolBar()
  */
void APrivateWindow::removeToolBar( QString in )
    {
    delete _toolBars.take( in );
    }

/** Return the document active in the application ( there is only ever one )
  */
APrivateDocument &APrivateWindow::document()
    {
    return _document;
    }

/** \overload
  */
const APrivateDocument &APrivateWindow::document() const
    {
    return _document;
    }

/** Display a status message on the bottom bar of the application
  */
void APrivateWindow::statusMessage( QString text, unsigned int time )
    {
    if( ui->statusbar )
        {
        ui->statusbar->showMessage( text, time );
        }
    }

/** Set the active file for the application
  */
void APrivateWindow::setActiveFile( QString in, QString ext )
    {
    _currentFilename = in;
    _currentExtension = ext;
    refreshWindowTitle();
    }

/** Refresh the title of the window, called when the active file has changed.
  */
void APrivateWindow::refreshWindowTitle()
    {
    // if we have a filename
    if( _currentFilename != "" )
        {
        setWindowTitle( _currentFilename + "[*] - " + document().programName() );
        }
    else
        {
        // else use a default name
        setWindowTitle( tr( "Unsaved Document" ) + "[*] - " + document().programName() );
        }
    }

/** Called before the create menu is clicked to refresh the list of items the user can create
  */
void APrivateWindow::rebuildCreateMenu( )
    {
    if( ui->menuCreate )
        {
        // empty the menu
        ui->menuCreate->clear();

        // enumerate groups
        QHash <QString, QMenu *> menus;
        foreach( AEntityType key, document().entityCreators().keys() )
            {
            // find if the type is shown
            const APrivateEntityCreatorFn &value( document().entityCreators().value( key ) );
            if( value.show == TRUE )
                {
                // if the type has a group and we didnt already add its menu
                if( key.group() != "" && menus[key.group()] == 0 )
                    {
                    // find its drawable name, incase the programmer used /group/wrong/name
                    QStringList drawName = key.group().split( "/", QString::SkipEmptyParts );
                    // add a menu for it to the list
                    menus[key.group()] = new QMenu( drawName.back(), ui->menuCreate );
                    // add to the create menu
                    ui->menuCreate->addMenu( menus[key.group()] );
                    }
                }
            }
        // now for each type
        foreach( AEntityType key, document().entityCreators().keys() )
            {
            //find if it should be shown
            const APrivateEntityCreatorFn &value( document().entityCreators().value( key ) );
            if( value.show == TRUE )
                {
                // either add it to the create menu directly
                if( key.group() == "" )
                    {
                    ui->menuCreate->addAction( key.name() )->setData( key.hash() );
                    }
                else
                    {
                    // or add it to the sub menu
                    menus[key.group()]->addAction( key.name() )->setData( key.hash() );
                    }
                }
            }
        }
    }

/** Slot used for when a create menu item is triggered
  \sa rebuildCreateMenu()
  */
void APrivateWindow::createTriggered( QAction *cr )
    {
    // for each type
    foreach( AEntityType key, document().entityCreators().keys() )
        {
        // if this type is the type triggered
        const APrivateEntityCreatorFn &value( document().entityCreators().value( key ) );
        if( cr->data().toString() == key.hash() )
            {
            // create an entity
            AEntity *ent = (*value.fn)( );

            // add it to the array
            document().addEntity( ent );
            return;
            }
        }
    }

/** Called before the file menu is shown
  */
void APrivateWindow::rebuildFileMenu( )
    {
    // enable save ( or not )
    ui->actionSave->setEnabled( isWindowModified() && _currentFilename != "" );

    // create recent files menu from user settings file
    QSettings settings;
    QVariantList recent( settings.value( "recentFiles" ).toList() );
    ui->menuRecent_Files->clear();
    foreach( QVariant r, recent )
        {
        ui->menuRecent_Files->addAction( r.toString() );
        }
    ui->menuRecent_Files->setEnabled( ui->menuRecent_Files->actions().size() );
    }

/** Sets the central dock to \p dock.
  \note The dock passed should already be an added dock to the application
  \sa addDockedItem()
  */
void APrivateWindow::setCentralDock( AAbstractDock *dock )
    {
    // Get rid of the central dock already installed
    clearCentralDock();

    // set the central widget
    setCentralWidget( dock->base() );

    // find the old QDockWidget in our list
    QDockWidget *oldDock( _qDocks[dock->name()] );
    if( oldDock )
        {
        // delete it
        oldDock->deleteLater();
        _qDocks.remove( dock->name() );
        }
    // save for later deletion or storing
    _centralDock = dock;
    }

/** Set the central dock to the dock with its context menu currently open.
  \todo I know this is a bad way to do this, but its the only way I could think of at the time. anyway, this class is internal anyway.
  */
void APrivateWindow::setCentralDock()
    {
    // if there is a context menu open
    if( _currentContextDock )
        {
        // set it as the central dock
        setCentralDock( _currentContextDock );
        }
    }

/** Clear the central dock.
  \sa setCentralDock()
  */
void APrivateWindow::clearCentralDock( )
    {
    // we cant clear it if its empty
    if( _centralDock )
        {
        // create a new QDockWidget for the central dock
        APrivateCustomDockWidget *dock( new APrivateCustomDockWidget( _centralDock, this ) );
        // add the dock widget
        addDockWidget( Qt::LeftDockWidgetArea, dock );
        // store the dock
        _qDocks[_centralDock->name()] = dock;
        // store new state
        _centralDock = 0;
        // make sure the QMainWindow knows its now empty
        setCentralWidget( 0 );
        }
    }

/** Called to refresh the window menu before it is shown.
  */
void APrivateWindow::rebuildWindowMenu( )
    {
    //clear the menu
    ui->menuWindow->clear();

    // if we have a context dock, we are going to be displayed somewhere other than from the top menu, so show a make central action
    if( _currentContextDock )
        {
        _currentContextDock->menu( ui->menuWindow );
        ui->menuWindow->addAction( "Make This Dock Central", this, SLOT(setCentralDock()) );
        ui->menuWindow->addSeparator();
        }

    // add an action for each dock
    foreach( QString key, _qDocks.keys() )
        {
        ui->menuWindow->addAction( _qDocks.value( key )->toggleViewAction() );
        }

    ui->menuWindow->addSeparator();

    // add a full screen option
    QAction *fS = ui->menuWindow->addAction( tr( "Full Screen" ) );
    fS->setShortcut( QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_F ) );
    fS->setCheckable( TRUE );
    fS->setChecked( windowState() & Qt::WindowFullScreen );

    // and the workspace menu
    ui->menuWindow->addMenu( _workspace->menu() );
    }

/** Called when a window menu action is triggered
  \sa rebuildWindowMenu()
  */
void APrivateWindow::pollWindowMenuStates( QAction *act )
    {
    // if fullscreen
    if( act->text() == tr( "Full Screen" ) )
        {
        setWindowState( windowState() ^ Qt::WindowFullScreen );
        }
    else
        {
        // otherwise try to show or hide a dock
        foreach( QString name, _dockedWidgets.keys() )
            {
            // find the docks name, compare to the triggered action text
            AAbstractDock *value( _dockedWidgets.value( name ) );
            if( value->name() == act->text() )
                {
                // hide or show the dock
                QDockWidget *dock( _qDocks[value->name()] );
                if( dock )
                    {
                    dock->setVisible( act->isChecked() );
                    }
                }
            }
        }
    }

/** Triggered to create a warning messagebox
  */
void APrivateWindow::handle( XString message )
    {
    // split to find title and message
    QStringList list = message.split( '|' );
    if( list.size() >= 2 )
        {
        QMessageBox::warning(this, list[0], list[1] );
        }
    else
        {
        // use warning as title instead of custom title
        QMessageBox::warning(this, "Warning", message );
        }
    ALog << message;
    }

/** Slot to create a new file
  */
void APrivateWindow::fileNew()
    {
    // if the user doesnt want their changes
    if( discardChanges() )
        {
        document().newFile();
        }
    }

/** Slot to open a file
  */
void APrivateWindow::fileOpen()
    {
    // if the user doesnt want their changes ( or saves them )
    if( discardChanges() )
        {
        // if we have a file plugin installed
        QString extensions = document().fileFilterList();
        if( extensions != "" )
            {
            // create a file dialog
            QString ext;
            QString file( AFileDialog::getOpenFileName( "", extensions, &ext ) );

            // run the dialog
            if( file != "" && QFile::exists( file ) )
                {
                // try to open it
                document().openFile( file, ext );
                setActiveFile( file, ext );
                addRecentFile( _currentFilename );
                }
            else
                {
                AWarning<<"File Error|No file found for opening."<<endl;
                }
            }
        else
            {
            AWarning<<"File Format Error|No file types found, opening not possible."<<endl;
            }
        }
    }

/** Slot for saving to the previously used file
  */
void APrivateWindow::fileSave()
    {
    // if we can save ( we need a filename )
    if( _currentFilename != "" )
        {
        // try to save
        document().saveFile( _currentFilename, _currentExtension );
        addRecentFile( _currentFilename );
        }
    else
        {
        // otherwise try a saveas
        fileSaveAs();
        }
    }

/** Slot for a saveas dialog
  */
void APrivateWindow::fileSaveAs()
    {
    // find the active extensions, we cant save without some
    QString extensions = document().fileFilterList();
    if( extensions != "" )
        {
        // create a file dialog
        QString ext;
        QString file( AFileDialog::getSaveFileName( "", extensions, &ext ) );

        // run the dialog
        if( file != "" )
            {
            if( QFileInfo(file).completeSuffix() == "" )
                {
                file += "." + document().findFileTypeFromFilterString( ext )->getDefaultSuffix();
                }
            // try to save
            document().saveFile( file, ext );
            setActiveFile( file, ext );
            addRecentFile( _currentFilename );
            }
        }
    else
        {
        AWarning<<"File Format Error|No file types found, saving not possible."<<endl;
        }
    }

/** Tests whether the user wants to discard their changes
  */
bool APrivateWindow::discardChanges( )
    {
    // if the window has changes
    if( isWindowModified() )
        {
        // see what the user wants to do
        QMessageBox msgBox;
        msgBox.setText(tr("The document has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
        msgBox.setDefaultButton( QMessageBox::Cancel );

        // get the users response
        int ret = msgBox.exec();

        // take action
        if( ret == QMessageBox::Yes )
            {
            fileSave();
            return TRUE;
            }
        else if( ret == QMessageBox::Cancel )
            {
            return FALSE;
            }
        else
            {
            return TRUE;
            }
        }
    return TRUE;
    }

/** Triggered when the window wants to close ( user clicked close or exit )
  */
void APrivateWindow::closeEvent( QCloseEvent *event )
    {
    // if the user doesnt want their changes
    if( discardChanges() )
        {
        // destory the workspace
        _workspace->uninitiate();

        //save the central dock name to the settings
        QSettings settings;
        if( _centralDock )
            {
            settings.setValue( "centralDock", _centralDock->name() );
            }
        else
            {
            settings.remove( "centralDock" );
            }
        // save info about the window state
        settings.setValue( "windowState", (int)windowState() );
        settings.setValue( "windowGeometry", geometry() );
        event->accept();
        }
    else
        {
        // force a halt on the close event
        event->ignore();
        }
    }

/** Returns the file menu
  */
QMenu *APrivateWindow::fileMenu()
    {
    return ui->menuFile;
    }

/** Returns the window menu
  */
QMenu *APrivateWindow::windowMenu()
    {
    return ui->menuWindow;
    }

/** Returns the create menu
  */
QMenu *APrivateWindow::createMenu()
    {
    return ui->menuCreate;
    }

/** Sets the context dock, used when docks utilise the main windows window menu.
  */
void APrivateWindow::setContextDock( AAbstractDock *in )
    {
    _currentContextDock = in;
    }

/** Called when the modified status of the document changes, to update the window indicator
  */
void APrivateWindow::modifiedChanged( bool in )
    {
    setWindowModified( in );
    }

/** Add a recent file to the list
  */
void APrivateWindow::addRecentFile( QString file )
    {
    // find the current recent files
    QList <QVariant> recent;
    QSettings settings;
    recent = settings.value( "recentFiles" ).toList();

    // Remove the first item on the list
    while( recent.size() > ( RECENT_FILE_LIST_SIZE - 1) )
        {
        recent.takeFirst();
        }

    // Append to the list
    if( !recent.contains( file ) )
        {
        recent << file;
        }

    // Save the file
    settings.setValue( "recentFiles", recent );
    }

/** Triggered to open a recent file
  */
void APrivateWindow::openRecent( QAction *act )
    {
    QString ext;
    document().openFile( act->text(), ext );
    setActiveFile( act->text(), ext );
    }

/** Triggered when the user cuts
  */
void APrivateWindow::cut()
    {
    AEntitySearch sel( document().selection() );
    // only cut if there is a selection
    if( sel.size() != 0 )
        {
        QString cut( QString::fromUtf8( document().cut( sel ) ) );
        QApplication::clipboard()->setText( cut, QClipboard::Clipboard );
        }
    }

/** Triggered when the user copys
  */
void APrivateWindow::copy()
    {
    AEntitySearch sel( document().selection() );
    // only copy if there is a selection
    if( sel.size() != 0 )
        {
        QString copy( QString::fromUtf8( document().copy( sel ) ) );
        QApplication::clipboard()->setText( copy, QClipboard::Clipboard );
        }
    }

/** Triggered when the user pastes
  */
void APrivateWindow::paste()
    {
    QByteArray clipboard( QApplication::clipboard()->text().toUtf8() );
    document().paste( clipboard );
    }

/** Triggered from the delete action
  */
void APrivateWindow::deleteSelection()
    {
    document().removeEntities( document().selection() );
    }

/** Triggered for all events that occur in the application, we use it to group commands that happen inside mouse drags.
  */
bool APrivateWindow::eventFilter( QObject *obj, QEvent *event )
    {
    static bool blockBegun( false );
    if( ( event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick ) && !blockBegun )
        {
        document().undoStack().beginBlockChange();
        blockBegun = true;
        }
    else if( event->type() == QEvent::MouseButtonRelease && blockBegun )
        {
        blockBegun = false;
        document().undoStack().endBlockChange();
        }

    // Handle normally
    return QObject::eventFilter(obj, event);
    }

/** Returns the global SDK for use by users.
  */
APlugin *APrivateWindow::sdk()
    {
    return &_sdk;
    }

/** Adds this action to the edit menu.
  */
void APrivateWindow::addPreferencesAction( QAction *act )
    {
    ui->menuEdit->addAction( act );
    }

/** \endcond
  */
