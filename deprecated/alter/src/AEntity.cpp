#include "../SDK/AEntity"
#include "QString"
#include "APrivateWindow.h"
#include "../SDK/AEntitySearch"
#include "../SDK/ALog"

#include "QDebug"

/** \class AEntity
  \brief An AEntity provides an APropertyDevice with commands and controls that allow it to be stored in a direcotry structure.
  */

/** \fn AEntity::XProperty( AEntityArray *, array, setArray )
  This Property allows the user to set and get the array this property is in.
  \note It isnt recommended that the user changes the array pointer, but they may need to access and use the array.
  */

/** \fn AEntity::createInstance()
  This function is implemented by the A_ENTITY macro, it should be used in every class that inherits from AEntity, directly or indirectly, much like the Q_OBJECT macro.
  */

/** \fn AEntity::type() const
  This function is implemented by the A_ENTITY macro, it returns the type of the entity as an AEntityType.
  */

/** Create an AEntity
  */
AEntity::AEntity( ) :  _array( 0 ), _oldLocation( "/NULL" )
    {
    addProperty( "entity/name", _oldLocation.name(), propertyInfo::String, "Name" );
    addProperty( "entity/directory", _oldLocation.directory(), propertyInfo::String, "Directory" );
    }

/** Destroy the Entity, removeing it from its array
  */
AEntity::~AEntity( )
    {
    if( _array )
        {
        _array->remove( *this );
        }
    }

/** Copy this entity and return it.
  */
AEntity *AEntity::copy()
    {
    AEntity *ret( createInstance() );
    ret->properties( properties() );
    return ret;
    }

/** Find this Entitys name
  */
QString AEntity::name() const
    {
    return location().name();
    }

/** Find this entitys location
  */
AEntityLocation AEntity::location( ) const
    {
    return AEntityLocation( property( "entity/directory" )->toString() + "/" + property( "entity/name" )->toString() );
    }

/** Function allowing a sub-class to be allerted when a child is added or removed to this entity
  \note This function may become deprecated.
  */
void AEntity::childEvent()
    {
    }

/** Set this entitys parent to \p p
  */
void AEntity::setParent( AEntityLocation p )
    {
    // find my possible new location
    AEntityLocation newLoc = AEntityLocation( p.path(), name() );
    // check we are actually moving
    if( newLoc != _oldLocation )
        {
        // find all entities
        AEntitySearch all = array()->search();
        // find my parent
        AEntitySearch newParent = all.findEntity( p );
        // check that either i was just created, or my parent does exist, or im being moved to root
        // ( doesn't technically exist, but it is valid ).
        // Also check that there isnt already an entity at my new location.
        if( _oldLocation.path() == "/NULL" ||
            ( ( newParent.size() == 1  || AEntityLocation( p.path() ).isRoot() ) &&
            all.findEntity( newLoc ).size() == 0 ) )
            {
            // find my current children
            AEntitySearch children = array()->search().findDirectChildren( _oldLocation );

            // my old parent
            AEntitySearch pa = all.findEntity( _oldLocation.directory() );
            if( pa.size() )
                {
                // inform of its children change
                pa[0].childEvent();
                }

            // set my path
            property( "entity/directory" ) = p.path();

            // find my new parent
            pa = all.findEntity( parent() );
            if( pa.size() )
                {
                // inform of its children change
                pa[0].childEvent();
                }

            // parent my old children to me!
            for( int x=0; x<children.size(); x++ )
                {
                children[x].setParent( location() );
                }
            }
        else
            {
            ALog<<"There is a problem setting '"<<p.path()<<"' as the parent for '"<<location().path()<<"'"<<endl;
            }
        }
    }

/** Find my parent entity.
  */
AEntityLocation AEntity::parent( ) const
    {
    return AEntityLocation( location().directory() );
    }

/** Called when the entity is selected or unselected, so the entity can act.
  */
void AEntity::selectionEvent( ASelectionEvent * )
    {
    }

/** Set my name to \p in
  */
void AEntity::setName( QString in )
    {
    property( "entity/name" ) = in;
    }

/** Check my name is valid
  */
void AEntity::validateName( )
    {
    // get the new name
    QString newName( name() );

    // ensure it only has valid characters
    newName = newName.replace( QRegExp( "[^A-Za-z0-9_ -]" ), "_" );

    // if its actually changes from the last name i had
    AEntityLocation newLoc = AEntityLocation( location().directory(), newName );
    if( newName != _oldName || newLoc != location() )
        {
        // check there isnt already an entity called my new name
        AEntitySearch all = array()->search();

        if( all.findEntity( newLoc ).size() == 0 || &(all.findEntity( newLoc )[0]) == this )
            {
            // find my current children
            AEntitySearch children = all.findDirectChildren( _oldLocation );

            // set my name
            if( newName != name() )
                {
                setName( newName );
                }

            // reparent my children
            for( int x=0; x<children.size(); x++ )
                {
                children[x].setParent( location() );
                }
            }
        else
            {
            // there was a problem, go back to the old name
            setName( _oldName );
            ALog<<"The name given is not unique."<<endl;
            }
        }
    _oldName = newName;
    }

/** Hide this entity
  */
void AEntity::hide()
    {
    addProperty( "entity/hide", TRUE );
    }

/** Unhide this entity
  */
void AEntity::unhide()
    {
    removeProperty( "entity/hide" );
    }

/** Find if this entity is hidden
  */
bool AEntity::hidden( ) const
    {
    return propertyExists( "entity/hide" );
    }

/** Find if my location is valid
  */
bool AEntity::parentTreeIsValid()
    {
    xAssert( _array );
    return _array->pathIsValid( location() );
    }

/** Called when an entity is added to an array.
  */
void AEntity::addedToArray()
    {
    AEntityLocation loc( array()->getUniquePath( array()->workingPath(), type().name() ) );
    setParent( loc.directory() );
    setName( loc.name() );
    }

/** Called when an entity has been moved, for example cut and pasted, or saved and loaded. Intended to be used to attach non-persistant properties to entites etc.
  */
void AEntity::relocated()
    {
    _oldName = name();
    _oldLocation = location();
    }

/** Called once an entity has been removed from an array, for example copied, and needs to be wiped clean of all external links before beign used again.
    \p entityMap holds the names of old nodes connections are still allowed to in \p keys(), and their new names in the
    array at this point in time in \p values(). The programmer should disconnect any links to any node not contained in
    \p keys(), and re-link any connections in \p keys() to the associated \p value().
  */
void AEntity::dislocate( QHash <AEntityLocation, AEntityLocation> & )
    {
    }

/** Checks if the name was changed validates it, then calls APropertyDevice::prePropertyChanged()
  */
void AEntity::prePropertyChanged( AProperty *in )
    {
    if( in->name() == "entity/name" )
        {
        validateName();
        }
    APropertyDevice::prePropertyChanged( in );
    }
