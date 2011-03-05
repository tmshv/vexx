#include "../SDK/APlugin"
#include "APrivateWindow.h"
#include "../SDK/ALog"
#include "APrivatePropertyWidgetFactory.h"

/** \class APlugin
  \brief The \c APlugin class is a plugins reference into the application, it can install GUI elements and \c AEntity types.
  */

/** \fn APlugin::workingPathChanged()
  Triggered when the application working path changes
  */
/** \fn APlugin::insertState()
  Triggered when a paste style operation has occured ( where entities have been inserted spontaneously into the array ).
  */

/** \fn APlugin::cleanState()
  Triggered when a new document is created.
  */

/** \fn APlugin::openState()
  Triggered when a document has been opened.
  */

/** \fn APlugin::entityAdded( AEntity * )
  Triggered when an entity has been added to the array.
  */

/** \fn APlugin::entityRemoved( AEntity * )
  Triggered when an entity is removed from the array.
  */

/** \fn APlugin::selectionChanged()
  Triggered when the selection changes in the application
  */

/** \fn APlugin::accessAs( QString in )
  Access a core from the plugin \p in as type T.
  \deprecated
  */

/** Destroys this plugin
  */
APlugin::~APlugin( )
    {
    }

/** Add a plugins core pointer to the application.
  \deprecated
  \sa ASettings
  */
void APlugin::addCore( void *in )
    {
    xAssert( _controller );
    return _controller->pluginManager()->setCurrentCore( in );
    }

/** Request a plugin called \p in is loaded before this plugin continues.
  \return true if the request succeeded, otherwise false.
  */
bool APlugin::request( QString in )
    {
    xAssert( _controller );
    return _controller->pluginManager()->request( in );
    }

/** Get the core pointer for the plugin \p in.
  \deprecated
  \return the pointer to the value
  \sa ASettings
  \sa addCore()
  */
void *APlugin::access( QString in )
    {
    xAssert( _controller );
    return _controller->pluginManager()->access( in );
    }

/** Sets this plugin equal to \p in.
  \internal
  */
APlugin &APlugin::operator=( const APlugin &in )
    {
    _controller = in._controller;
    return *this;
    }

/** Created an APlugin from \p cnt.
  \internal
  \sa appBase
  */
APlugin::APlugin( APrivateWindow *cnt )
    {
    _controller = cnt;
    // connect my signals to the signals from appBase
    connect( &(cnt->document().propertyEntities()), SIGNAL(workingPathChanged()), this, SIGNAL(workingPathChanged()) );
    connect( &(cnt->document()), SIGNAL(insertState()), this, SIGNAL(insertState()) );
    connect( &(cnt->document()), SIGNAL(cleanState()), this, SIGNAL(cleanState()) );
    connect( &(cnt->document()), SIGNAL(openState()), this, SIGNAL(openState()) );
    connect( &(cnt->document()), SIGNAL(selectionChanged()), this, SIGNAL(selectionChanged()) );
    connect( &(cnt->document().propertyEntities()), SIGNAL(entityAdded(AEntity*)), this, SIGNAL(entityAdded(AEntity*)) );
    connect( &(cnt->document().propertyEntities()), SIGNAL(entityRemoved(AEntity*)), this, SIGNAL(entityRemoved(AEntity*)) );
    }

/** Install a creator type to the app.
  \param t The type of the entity to add
  \param f The function to create an entity of this type
  \param hide Whether the UI should display this type
  */
void APlugin::addCreator( AEntityType t, AEntityCreatorFn f, bool hide )
    {
    xAssert( _controller );
    _controller->document().addCreator( t, f, hide );
    }

/** Remove a creator type to the app.
  \param t The type of the entity to remove
  */
void APlugin::removeCreator( AEntityType t )
    {
    xAssert( _controller );
    _controller->document().removeCreator( t );
    }

/** Send the string \p str to the application log
  \sa ALog
  */
void APlugin::log( QString str )
    {
    xAssert( _controller );
    ALog<<str<<endl;
    }

/** Create a search containing all the entities in the application
  \return A search object with all the entities in the application in it.
  */
AEntitySearch APlugin::search()
    {
    xAssert( _controller );
    return AEntitySearch( _controller->document().propertyEntities() );
    }

/** Create a search containing all the entities in the application
  \return A search object with all the entities in the application in it.
  */
AEntitySearchConst APlugin::search() const
    {
    xAssert( _controller );
    return AEntitySearchConst( _controller->document().propertyEntities() );
    }

/** Insert an entity \p ent into the application.
  \note The entity may be renamed.
  */
void APlugin::addEntity( AEntity *ent )
    {
    xAssert( _controller );
    _controller->document().propertyEntities().add( *ent );
    }

/** Remove an entity \p ent from the application.
  \note the entity will not be deleted immediately, but the application will manage and delete it at an appropriate time.
  */
void APlugin::removeEntity( AEntity *ent )
    {
    xAssert( _controller );
    _controller->document().propertyEntities().remove( *ent );
    }

/** Add a dock \p dock to the application, with the name \p str
  \note A dock doesn't correspond directly to a Qt dock widget.
  */
void APlugin::addDockedItem( QString str, AAbstractDock *dock )
    {
    xAssert( _controller );
    _controller->addDockedItem( str, dock );
    }

/** Remove a docked item \p str from the application
  */
void APlugin::removeDockedItem( QString str )
    {
    xAssert( _controller );
    _controller->removeDockedItem( str );
    }

/** Add a toolbar \p tool to the application, with name \str
  */
void APlugin::addToolBar( QString str, AAbstractToolBar *tool )
    {
    xAssert( _controller );
    _controller->addToolBar( str, tool );
    }

/** Remove a toolbar \p str from the application
  */
void APlugin::removeToolBar( QString str )
    {
    xAssert( _controller );
    _controller->removeToolBar( str );
    }

/** Install a file type \p fileType to the application, called \p str
  */
void APlugin::addFileType( QString str, AAbstractFileType *fileType )
    {
    xAssert( _controller );
    _controller->document().addFileType( str, fileType );
    }

/** Remove a file type \p str from the application
  */
void APlugin::removeFileType( QString str )
    {
    xAssert( _controller );
    _controller->document().removeFileType( str );
    }

/** Triggeres a status bar message \p str to appear on the status bar for \p msecs milli seconds.
  \note Logging a string automatically triggers a status message for 1000 milli seconds.
  */
void APlugin::statusMessage( QString str, unsigned int msecs )
    {
    _controller->statusMessage( str, msecs );
    }

/** Clear the application selection
  */
void APlugin::selectClear()
    {
    xAssert( _controller );
    _controller->document().deselect();
    }

/** Select the entity \p ent
  \note the previous selection will be cleared
  */
void APlugin::select( AEntity *ent )
    {
    xAssert( _controller );
    _controller->document().select( ent );
    }

/** Add \p ent to the current selection
  */
void APlugin::selectAdd( AEntity *ent )
    {
    xAssert( _controller );
    _controller->document().selectAdd( ent );
    }

/** Forces all undo steps created until \ref endBlockChange() is called to be appended to the same undo action.
  \sa endBlockChange()
  */
void APlugin::beginBlockChange()
    {
    xAssert( _controller );
    _controller->document().undoStack().beginBlockChange( );
    }

/** Stops appending undo steps to the same undo action
  \sa beginBlockChange()
  */
void APlugin::endBlockChange()
    {
    xAssert( _controller );
    _controller->document().undoStack().endBlockChange();
    }

/** Find the current application selection in a search object
  */
AEntitySearch APlugin::selection( ) const
    {
    xAssert( _controller );
    return _controller->document().selection();
    }

/** Add a gui control type. This is a type used to create controls for properties. \p in can be any number from propertyInfo::UserType, or a non-user type to override a built in type.
  */
void APlugin::addGuiControlType( int in, guiPropertyTypeFunction type, guiPropertyValidatorFunction val )
    {
    APrivatePropertyWidgetFactory::registerType( in, type, val );
    }
/** Remove a Gui control type. Used to create custom conrols for properties. \p in should be the same as the id used to register the type.
  */
void APlugin::removeGuiControlType( int in )
    {
    APrivatePropertyWidgetFactory::unregisterType( in );
    }

/** The working path for the application, where entities will be inserted etc.
  */
AEntityLocation APlugin::workingPath() const
    {
    xAssert( _controller );
    return _controller->document().propertyEntities().workingPath( );
    }

/** Set the working path for the application.
  \sa workingPath();
  */
void APlugin::setWorkingPath( AEntityLocation  path)
    {
    xAssert( _controller );
    _controller->document().propertyEntities().setWorkingPath( path );
    }

/** Returns whether the application is currently inserting, so operations such as rebuilding GUI elements can be optimised and not repeated for each entityAdded call.
  */
bool APlugin::inserting() const
    {
    xAssert( _controller );
    return _controller->document().inserting();
    }

/** Copys \p search into the string given.
  \note Also copies the children entities of search
  \sa APlugin::paste()
  */
QByteArray APlugin::copy( const AEntitySearch &search )
    {
    return _controller->document().copy( search );
    }

/** Paste the entities in \p data into the array
  \note pastes into the working directory
  \sa APlugin::copy()
  */
void APlugin::paste( QByteArray data )
    {
    xAssert( _controller );
    _controller->document().paste( data );
    }

/** Add an action intended to trigger application preferences
  */
void APlugin::addPreferencesAction( QAction *act )
    {
    xAssert( _controller );
    _controller->addPreferencesAction( act );
    }
