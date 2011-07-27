#include "APrivateDocument.h"
#include "APrivateCommand.h"
#include "APrivateCommandStack.h"
#include "APrivateSystemEntity.h"
#include "../SDK/ALog"
#include "QFileInfo"
#include "../SDK/ASelectionEvent"

#include "QDebug"

/** \cond INTERNAL
  */

/** \fn extendToChildren( AEntityArray *arr, const AEntitySearch &search )
  \brief Find children of each entity in the search in \p arr, and return
  \note The original entities are also included in the search
  \todo Maybe move this to a public function...? of search, or array???
  \relates APrivateDocument
  */
AEntitySearch extendToChildren( AEntityArray *arr, const AEntitySearch &search )
    {
    AEntitySearch ret;

    AEntitySearch all( *arr );

    foreach( AEntity *ent, search )
        {
        ret += all.findChildren( ent->location() ) + ent;
        }

    return ret;
    }

/** \class APrivateDocument
  \brief The \c appDocument class holds an array of entities and manages operations such as undo, redo and clipboard
  operations on them. It also saves and loads the to file.
  */

/** \fn APrivateDocument::insertState()
  Emitted when entites have been inserted into the array via paste or insert.
  */

/** \fn APrivateDocument::cleanState()
  Emitted when the document is set to a "new" state.
  */

/** \fn APrivateDocument::openState()
  Emitted when entites have been inserted into the array via open.
  */

/** \fn APrivateDocument::selectionChanged()
  Emitted when the array selection changes.
  */

/** \fn APrivateDocument::modified(bool)
  Emitted when the application modified state changes.
  */

/** Construct an \c APrivateDocument from strings about the program being run - these cause file loading and saving
  to become incompatible if changed - for example, two different docuemnts with \p appName "thing" and "thing2"
  would not be able to load each others files, even if the content and classes used were exactly the same.
  \todo \p version shouldn't disable loading, but should call an updating function... but right now v1.1 files dont load int v1.1.1
  */
APrivateDocument::APrivateDocument( QString appName, QString orgName, QString domainName, QString version )
        : _modified( false )
    {
    // create undo stack
    _undo = new APrivateCommandStack( this );

    // connect to the arrays command signal
    connect( &_propertyEntities, SIGNAL(onCommand(APrivateCommand**)), this, SLOT(onCommand(APrivateCommand**)) );

    // add a system entity
    APrivateSystemEntity *ent = new APrivateSystemEntity( );
    // setup system entity
    ent->setData( appName, domainName, orgName, version );
    // add to arrays
    _propertyEntities.add( *ent );

    // reset this for use later
    _inserting = FALSE;

    // connect to removed signal
    connect( &_propertyEntities, SIGNAL(entityRemoved(AEntity*)), this, SLOT(arrayChange(AEntity*)) );
    }

/** Destroy the application
  */
APrivateDocument::~APrivateDocument()
    {
    // order is important here. undo stack and array hold a delicate balance of entities.
    //clear the array
    propertyEntities().clear();

    // remove undo stack
    delete _undo;
    }

/** Add a creator to the document, the document then calls \p f whenever it needs to create a \p n. \p show
  controls whether the type is a private type - that should only be created by the programmer, not the user.
  */
void APrivateDocument::addCreator( AEntityType n, AEntityCreatorFn f, bool show )
    {
    APrivateEntityCreatorFn temp;
    temp.fn = f;
    temp.show = show;
    _propertyEntityCreators.insert( n, temp );
    }

/** Remove a creator \p t from the creation list.
  */
void APrivateDocument::removeCreator( AEntityType t )
    {
    _propertyEntityCreators.remove( t );
    }

/** Returns the creators currently available in the application
  */
const propertyEntityCreators &APrivateDocument::entityCreators() const
    {
    return _propertyEntityCreators;
    }

/** \overload
  */
propertyEntityCreators &APrivateDocument::entityCreators()
    {
    return _propertyEntityCreators;
    }

/** Triggered when the application receives a command from the \c AEntityArray. It is added to the undo stack.
  */
void APrivateDocument::onCommand( APrivateCommand **com )
    {
    // push the command
    _undo->push( *com );
    // set the pointer to zero so the sender knows weve used it
    *com = 0;
    // system is now modified
    setModified( TRUE );
    }

/** Add the entity to the current selection
  */
void APrivateDocument::selectAdd( AEntity *in )
    {
    // add to selection
    _selection.append( in );
    // inform slots
    Q_EMIT selectionChanged();

    ASelectionEvent ev( ASelectionEvent::Selected );
    in->selectionEvent( &ev );
    }

/** Select the entity. Deselects all other entities first.
  \sa selectAdd()
  */
void APrivateDocument::select( AEntity *in )
    {
    // deselect first
    _selection.clear();
    // select
    _selection.append( in );
    // inform
    Q_EMIT selectionChanged();

    ASelectionEvent ev( ASelectionEvent::Selected );
    in->selectionEvent( &ev );
    }

/** Select the entity. Deselects all other entities first.
  \sa selectAdd()
  */
void APrivateDocument::select( AEntitySearch in )
    {
    // deselect first
    _selection.clear();
    // select
    _selection = in;
    // inform
    Q_EMIT selectionChanged();

    foreach( AEntity *ent, in )
        {
        ASelectionEvent ev( ASelectionEvent::Selected );
        ent->selectionEvent( &ev );
        }
    }

/** Deselect all selected entities
  */
void APrivateDocument::deselect( AEntity *in )
    {
    // remove in from the selected list
    _selection.remove( in );
    // inform
    Q_EMIT selectionChanged();

    ASelectionEvent ev( ASelectionEvent::Unselected );
    in->selectionEvent( &ev );
    }

/** Deselect all entities
  */
void APrivateDocument::deselect( )
    {
    AEntitySearch sel( _selection );
    _selection.clear();
    Q_EMIT selectionChanged();

    foreach( AEntity *ent, sel )
        {
        ASelectionEvent ev( ASelectionEvent::Unselected );
        ent->selectionEvent( &ev );
        }
    }

/** Returns the applications undo stack
  */
APrivateCommandStack &APrivateDocument::undoStack()
    {
    return *_undo;
    }

/** \overload
  */
const APrivateCommandStack &APrivateDocument::undoStack() const
    {
    return *_undo;
    }

/** Add an entity to the array
  */
AEntityLocation APrivateDocument::addEntity( AEntity *ent )
    {
    // add and return the new location of the entity
    _propertyEntities.add( *ent );
    return ent->location();
    }

/** Remove the entity from the array
  */
void APrivateDocument::removeEntity( AEntity *in )
    {
    _propertyEntities.remove( *in );
    }

/** Return the organisation name of the document
  \todo Could use an \c ASettings here.
  */
QString APrivateDocument::organisationName( ) const
    {
    xAssert( _propertyEntities.search().findType( APrivateSystemEntity::identifier() ).size() );
    return _propertyEntities.search().findType( APrivateSystemEntity::identifier() )[0].property( "system/organisation" )->toString();
    }

/** Return the organisation domain of the document
  \todo Could use an \c ASettings here.
  */
QString APrivateDocument::organisationDomain( ) const
    {
    xAssert( _propertyEntities.search().findType( APrivateSystemEntity::identifier() ).size() );
    return _propertyEntities.search().findType( APrivateSystemEntity::identifier() )[0].property( "system/domain" )->toString();
    }

/** Return the program name of the document
  \todo Could use an \c ASettings here.
  */
QString APrivateDocument::programName( ) const
    {
    xAssert( _propertyEntities.search().findType( APrivateSystemEntity::identifier() ).size() );
    return _propertyEntities.search().findType( APrivateSystemEntity::identifier() )[0].property( "system/program" )->toString();
    }

/** Return the program version
  \todo Could use an \c ASettings here.
  */
QString APrivateDocument::programVersion( ) const
    {
    xAssert( _propertyEntities.search().findType( APrivateSystemEntity::identifier() ).size() );
    return _propertyEntities.search().findType( APrivateSystemEntity::identifier() )[0].property( "system/version" )->toString();
    }

/** Return the entity array.
  */
AEntityArray &APrivateDocument::propertyEntities()
    {
    return _propertyEntities;
    }

/** Save the docuemnt to \p fName with the given extension.
  \note extension must be an installed file type
  */
void APrivateDocument::saveFile( QString fName, QString ext )
    {
    // if we save successfully
    if( fillFileFromSearch( propertyEntities().search(), fName, ext ) )
        {
        // no longer modified
        setModified( FALSE );
        }
    else
        {
        // couldnt save
        AWarning<<"File Name Error|File name provided doesn't have a recognised file extension." << ext <<endl;
        }
    }

/** Open the file \p fName with the given extension.
  \note extension must be an installed file type or ""
  */
void APrivateDocument::openFile( QString fName, QString &ext )
    {
    // find an appropriate extension
    if( ext == "" )
        {
        // try to strip from filename
        QFileInfo info( fName );
        ext = findFileTypeFromExtension( info.completeSuffix() )->getFilterString();
        if( ext == "" )
            {
            // we couldnt find a file type
            AWarning<<"File Error|The File requested was an unrecognised format."<<endl;
            }
        }

    // ehnable insertion mode
    _inserting = TRUE;

    // try to open
    if( fillArrayFromFile( propertyEntities(), _propertyEntityCreators, fName, ext ) )
        {
        // disable insertion mode
        _inserting = FALSE;
        // inform about state
        Q_EMIT openState();
        // we arent modified now
        setModified( FALSE );
        }
    else
        {
        // opening failed
        AWarning<<"File Error|The File requested was an unrecognised format."<<endl;
        }
    // reset insertion mode in case we didnt insert
    _inserting = FALSE;
    }

/** Create a new empty document
  */
void APrivateDocument::newFile( )
    {
    // remove all entities except system entity
    for( unsigned int x=0; x<_propertyEntities.size(); x++ )
        {
        if( _propertyEntities[x].type() != APrivateSystemEntity::identifier() )
            {
            _propertyEntities.remove( _propertyEntities[x--] );
            }
        }
    // inform people
    Q_EMIT cleanState();
    // clear undo stack
    _undo->clear();
    // no longer modified
    setModified( FALSE );
    }

/** Triggered on an array remove command. Deselects \p in
  */
void APrivateDocument::arrayChange( AEntity *in )
    {
    // On en array remove we deselect the entity
    deselect( in );
    }

/** Sets the modified status of the document and emits a signal if it changes.
  */
void APrivateDocument::setModified( bool in )
    {
    // Test for change
    if( in != _modified )
        {
        // set and inform
        _modified = in;
        emit modified( in );
        }
    }

/** Remove \p in from the array.
    \note This function also removes the children of the entites.
  */
void APrivateDocument::removeEntities( const AEntitySearch &in )
    {
    AEntitySearch remove( extendToChildren( &(propertyEntities()), in ) );

    undoStack().beginBlockChange();
    // remove in loop
    foreach( AEntity *s, remove )
        {
        removeEntity( s );
        }
    undoStack().endBlockChange();
    }

/** Cuts \p search from the array into the string returned.
  \note Also cuts the children entities of search
  */
QByteArray APrivateDocument::cut( const AEntitySearch &search )
    {
    // call copy
    QByteArray ret = copy( search );
    // then remove entities
    removeEntities( search );
    // return copied string
    return ret;
    }

/** Copys \p search into the string given.
  \note Also copies the children entities of search
  */
QByteArray APrivateDocument::copy( const AEntitySearch &search )
    {
    // check we can export
    if( APrivateEntitySaver::isValid() )
        {
        // find all children
        AEntitySearch searchExtended( extendToChildren( &(propertyEntities()), search ) );
        // return the stripped values
        return APrivateEntitySaver::extract( searchExtended ).toHex();
        }
    // otherwise we couldnt copy cause of no file plugins
    AWarning << "Copy Error|No file types are available, clipboard functions are disabled";
    return "";
    }

/** Paste the string in \p data into the array
  \note pastes into the working directory
  */
void APrivateDocument::paste( QByteArray data )
    {
    // we must have a file loading plugin installed
    if( APrivateEntitySaver::isValid() )
        {
        // encompass in one undo event
        undoStack().beginBlockChange();

        AEntitySearch preInsert( _propertyEntities );

        _inserting = TRUE;
        // insert using the current application state
        APrivateEntitySaver::insert( QByteArray::fromHex( data ), _propertyEntities, _propertyEntityCreators );
        _inserting = FALSE;
        // emitpaste signal
        emit insertState();

        AEntitySearch postInsert( _propertyEntities );

        select( ( postInsert - preInsert ).findDirectChildren( _propertyEntities.workingPath() ) );

        // end undo
        undoStack().endBlockChange();
        return;
        }
    // couldnt load because of plugins
    AWarning << "Copy Error|No file types are available, clipboard functions are disabled";
    }

/** \endcond
  */
