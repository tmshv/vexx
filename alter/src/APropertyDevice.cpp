#include "../SDK/APropertyDevice"
#include "../SDK/ALog"
#include "APrivateDeviceCommand.h"
#include "APrivateCommandTools.h"
#include "XDebug"

/** \class APropertyDevice
  \brief An APropertyDevice holds a list of properties and allows a user to search and iterate through them.
  It also provides signals regarding the updates and changes made to the properties.
  */

/** \fn APropertyDevice::entityChanged( APropertyDevice *, AProperty * )
  Emitted when a property in this device changes.
  */

/** \cond INTERNAL
  \fn APropertyDevice::onCommand( command ** )
  Emitted when this device receives a command, so the listener can catch and pass it on.
  */

/** Construct a property device.
  */
APropertyDevice::APropertyDevice() : _active( 0 )
    {
    }

/** Destroy the device, deleting all contained properties
  */
APropertyDevice::~APropertyDevice()
    {
    // chaged signal, now we could use the destroyed signal though... so not so useful???
    Q_EMIT entityChanged( this, 0 );

    // remove each property
    foreach( AProperty *p, _properties )
        {
        delete p;
        }
    _properties.clear();
    }

/** Remove a property from the device
  */
void APropertyDevice::removeProperty( QString n )
    {
    // emit a command, it will do all the actual work
    emitCommand( APrivateDeviceCommand( this, n, APrivateDeviceCommand::Remove ) );
    }

/** Add a property to the device
  */
void APropertyDevice::addProperty( QString n, AVariant v, int t, QString d )
    {
    // create a basic display name if it wasnt given
    if( d == "" )
        {
        QStringList li = n.split( '/', QString::SkipEmptyParts );
        d = li.back();
        }
    // emit a command, it will actually add the property when required
    emitCommand( APrivateDeviceCommand( this, n, APrivateDeviceCommand::Add, new AProperty( this, n, v, t, d ) ) );
    }

/** Find a property \p in and return it.
  \note Causes a crash if the property doesn't exist, use propertyExists() to check.
  \sa propertyExists()
  */
const AProperty &APropertyDevice::property( QString in ) const
    {
#ifndef QT_NO_DEBUG
    if( !propertyExists( in ) )
        {
        xError( "Property Missing " << in );
        }
#endif
    // search for property
    return *_properties.find( in ).value();
    }

/** \overload
  */
AProperty &APropertyDevice::property( QString in )
    {
#ifndef QT_NO_DEBUG
    if( !propertyExists( in ) )
        {
        xError( "Property Missing "<<in );
        }
#endif
    // search for property
    return *_properties.find( in ).value();
    }

/** Find if a property \p in exists
  */
bool APropertyDevice::propertyExists( QString in ) const
    {
    return _properties.find( in ) != _properties.end( );
    }

/** Returns the number of properties in the device
  \deprecated
  */
unsigned int APropertyDevice::numberOfProperties() const
    {
    return _properties.size();
    }

/** Find the name of a property from the display name
  \deprecated
  */
QString APropertyDevice::nameFromDisplayName( QString in ) const
    {
    foreach( QString p, _properties.keys() )
        {
        if( _properties.value( p )->displayName() == in )
            {
            return p;
            }
        }
    return "";
    }

/** Triggered when a property contained in this device is changed.
    This function is the recommended way to validate properties in child classes, by catching the change, validating the
    new value, and reseting to a held previous value if necesary, then not emitting the current changed signal (by not
    calling the parent prePropertyChange() function. The signal for the reset change will then call this function again,
    pass validation, and be emitted to children.
\todo Validation should be easier, some kind of QEvent::ignore() style dohicky.
  */
void APropertyDevice::prePropertyChanged( AProperty *in )
    {
    // forward on to our listeners
    Q_EMIT entityChanged( this, in );
    }

/** Set our entire property array from another array.
  */
void APropertyDevice::properties( const APropertyDevice::propertyMap &in )
    {
    // clear our properties first
    foreach( QString p, _properties.keys() )
        {
        removeProperty( p );
        }
    // copy each one in
    foreach( QString p, in.keys() )
        {
        AProperty *value( in.value( p ) );
        addProperty( p, value->value(), value->type(), value->displayName() );
        }
    }

/** Get the entire property array.
  */
const APropertyDevice::propertyMap &APropertyDevice::properties() const
    {
    return _properties;
    }

/** Clear the entire property array.
  \deprecated
  */
void APropertyDevice::clearProperties()
    {
    _properties.clear();
    }
