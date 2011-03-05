#include "../SDK/AEntityArray"
#include "../SDK/AEntitySearch"
#include "APrivateArrayCommand.h"
#include "../SDK/ALog"
#include "APrivateCommandTools.h"
#include "../SDK/ASettings"

/** \class AEntityArray
  \brief This class holds an array of AEntity classes and manages adding and removing entities to the array.
  */


/** \fn AEntityArray::workingPathChanged()
  Emitted when the application working path changes.
  */

/** \fn AEntityArray::entityAdded(AEntity*)
  Emitted when an entity is added to the array.
  */

/** \fn AEntityArray::entityRemoved(AEntity*)
  Emitted when an entity is removed from the array.
  */

/** \cond INTERNAL
  \fn AEnityArray::onCommand(command**)
  Emitted when an array receives a command or creates one.
  \endcond
  */

/** Create an array
  */
AEntityArray::AEntityArray( )
    {
    }

/** Destroy the array
  */
AEntityArray::~AEntityArray( )
    {
    foreach( AEntity *e, _entities )
        {
        delete e;
        }
    }

/** Test if the path in \p loc is valid in the array ( all the parents are in place
  */
bool AEntityArray::pathIsValid( AEntityLocation loc )
    {
    // path is null
    if( loc.path() == "" )
        {
        return TRUE;
        }


    AEntitySearch all( *this );

    // try to find each level, if we cant, it isn't valid
    for( unsigned int x=0; x<loc.level(); x++ )
        {
        if( all.findEntity( loc.upToLevel( x ) ).size() == 0 )
            {
            return FALSE;
            }
        }

    // must be valid
    return TRUE;
    }

/** The working directory of the application, where entities are created, and where things are pasted.
  */
AEntityLocation AEntityArray::workingPath() const
    {
    ASettings settings( this, "ArraySettings" );
    return AEntityLocation( settings.value( "workingPath", "/" ).toString() );
    }

/** Set the working directory of the application, this is where entites are created and pasted.
  */
void AEntityArray::setWorkingPath( AEntityLocation in )
    {
    ASettings settings( this, "ArraySettings" );
    settings.setValue( "workingPath", in );
    Q_EMIT workingPathChanged();
    }

/** Find if a path is unique
  */
bool AEntityArray::pathIsUnique( AEntityLocation in )
    {
    // try to find the path
    AEntitySearch all( *this );
    return all.findEntity( in ).size() == 0;
    }

/** Find a list of all the Entities in the array
  */
const entityList &AEntityArray::entities()
    {
    return _entities;
    }

/** Get a unique path for an entity with parent \p parent, and name beginning \p nameBegin.
  */
AEntityLocation AEntityArray::getUniquePath( AEntityLocation parent, QString nameBegin )
    {
    // find an initial path
    QString path( parent.path() + "/" + nameBegin );
    // if this path isnt unique
    if( !pathIsUnique( path ) )
        {
        // add numbers to the end se we can create a unique one
        unsigned int num = 1;
        do
            {
            path = parent.path() + "/" + nameBegin + QString::number( num );
            num++;
            } while( !pathIsUnique( path ) );
        }
    return path;
    }

/** Add the entity \p in to the array.
  \todo this should accept a pointer
  */
void AEntityArray::add( AEntity &in )
    {
    // emit a command
    emitCommand( APrivateArrayCommand( this, &in, APrivateArrayCommand::Add ) );
    }

/** Remove an entity from the array.
  \todo this should accept a pointer
  */
void AEntityArray::remove( AEntity &in )
    {
    emitCommand( APrivateArrayCommand( this, &in, APrivateArrayCommand::Remove ) );
    }

/** Clear the array.
  */
void AEntityArray::clear()
    {
    while( _entities.size() )
        {
        // remove each entity, causing undo steps
        remove( *_entities.front( ) );
        }
    }

/** Find the number of entities in the array
  */
unsigned int AEntityArray::size() const
    {
    return _entities.size();
    }

/** Create a search from the array.
  \deprecated
  */
AEntitySearch AEntityArray::search()
    {
    return AEntitySearch( *this );
    }

/** Create a search from the array.
  \deprecated
  */
AEntitySearchConst AEntityArray::search() const
    {
    return AEntitySearchConst( *this );
    }

/** Find the entity at index \p x.
  */
AEntity &AEntityArray::operator[]( unsigned int x )
    {
    return *(_entities[x]);
    }

/** \overload
  */
const AEntity &AEntityArray::operator[]( unsigned int x ) const
    {
    return *(_entities[x]);
    }
