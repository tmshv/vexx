#include "../SDK/ASettings"
#include "../SDK/APlugin"
#include "../SDK/AVariant"
#include "APrivateSettingsEntity.h"

/** \class ASettings
  \brief ASettings provides access to an apparently persistant settings array stored in the currently open file.
  The programmer can query for values without worrying about entity or property creation or inheritance.
  */

/** Creates an ASettings object in the plugin \p app, giving it the unique name \p name.
    If the entity already exists in the array we attach to it, otherwise we create an entity.
  \note If \p app already has an entity called \p name which wasn't created by an \c ASettings class, this function will fail.
  */
ASettings::ASettings( APlugin *app, QString name ) : _constEntity( 0 )
    {
    // try to find the entity
    AEntitySearch search( app->search().findEntity( name ) );
    // test our search
    if( search.size() && search[0].type() == APrivateSettingsEntity::identifier() )
        {
        // attach if we found the entity
        _entity = &search[0];
        }
    else if( search.size() == 0 ) // we need to create an entity
        {
        // create and add to array
        _entity = APrivateSettingsEntity::create();
        app->addEntity( _entity );
        // set the name
        _entity->setName( name );
        // hide entity
        _entity->hide();
        // entity should now be named name
        if( _entity->name() != name )
            {
            AWarning << "Entity couldn't be named correctly";
            app->removeEntity( _entity );
            }
        }
    else
        {
        // another entity exists called name...
        AWarning << "Entity " << name << " exists, but isn't a settings entity";
        }
    }

/** \overload
    Creates an ASettings object in the plugin \p app, giving it the unique name \p name.
    If the entity already exists in the array we attach to it, otherwise we create an entity.
  \note This constructor creates a read only settings querier, and cannot set or ensure values, only return previously set ones.
  \note If \p app already has an entity called \p name which wasn't created by an \c ASettings class, this function will fail.
  */
ASettings::ASettings( const APlugin *app, QString name ) : _entity( 0 )
    {
    // try to find the entity
    AEntitySearchConst search( app->search().findEntity( name ) );
    // test our search
    if( search.size() && search[0].type() == APrivateSettingsEntity::identifier() )
        {
        // attach if we found the entity
        _constEntity = &search[0];
        }
    else
        {
        _constEntity = 0;
        }
    }

/** Creates an ASettings object in the array \p array, giving it the unique name \p name.
    If the entity already exists in the array we attach to it, otherwise we create an entity.
  \note If \p app already has an entity called \p name which wasn't created by an \c ASettings class, this function will fail.
  */
ASettings::ASettings( AEntityArray *array, QString name ) : _constEntity( 0 )
    {
    // try to find the entity
    AEntitySearch search( array->search().findEntity( name ) );
    if( search.size() && search[0].type() == APrivateSettingsEntity::identifier() )
        {
        // attach if we found the entity
        _entity = &search[0];
        }
    else if( search.size() == 0 )
        {
        // create and add to array
        _entity = APrivateSettingsEntity::create();
        array->add( *_entity );
        // set the name
        _entity->setName( name );
        // hide entity
        _entity->hide();
        // entity should now be named name
        if( _entity->name() != name )
            {
            AWarning << "Entity couldn't be named correctly";
            array->remove( *_entity );
            }
        }
    else
        {
        // another entity exists called name...
        AWarning << "Entity " << name << " exists, but isn't a settings entity";
        }
    }

/** \overload
    Creates an ASettings object in the array \p array, giving it the unique name \p name.
    If the entity already exists in the array we attach to it, otherwise we create an entity.
  \note This constructor creates a read only settings querier, and cannot set or ensure values, only return previously set ones.
  \note If \p app already has an entity called \p name which wasn't created by an \c ASettings class, this function will fail.
  */
ASettings::ASettings( const AEntityArray *array, QString name ) : _entity( 0 )
    {
    // try to find the entity
    AEntitySearchConst search( array->search().findEntity( name ) );
    if( search.size() && search[0].type() == APrivateSettingsEntity::identifier() )
        {
        // attach if we found the entity
        _constEntity = &search[0];
        }
    else
        {
        _constEntity = 0;
        }
    }

/** Find the value of the key \p in, or returns \p var if the key doesnt exist
  \sa ensureValue()
  */
AVariant ASettings::value( QString in, AVariant var ) const
    {
    // ensure the entity exists, and the property too
    if( _entity && _entity->propertyExists( in ) )
        {
        // return found property
        return _entity->property( in );
        }
    if( _constEntity && _constEntity->propertyExists( in ) )
        {
        // return found property
        return _constEntity->property( in );
        }
    // return default
    return var;
    }

/** Returns the value for the key \p in, and if the doesn't exist adds it with the value \p var.
  \sa value()
  */
AVariant ASettings::ensureValue( QString in, AVariant var )
    {
    // attach if we found the entity
    if( _entity && _entity->propertyExists( in ) )
        {
        return _entity->property( in );
        }
    if( _constEntity && _constEntity->propertyExists( in ) )
        {
        return _constEntity->property( in );
        }
    // if not then add the value
    setValue( in, var );
    // return default
    return var;
    }

/** Sets the value of the key \p in, to \p val
  */
void ASettings::setValue( QString in, AVariant val )
    {
    // make sure the property exists
    if( _entity )
        {
            if( !_entity->propertyExists( in ) )
            {
            _entity->addProperty( in, AVariant(), propertyInfo::String );
            }
        // set the value
        _entity->property( in ) = val;
        }
    }
