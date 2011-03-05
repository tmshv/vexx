#include "APrivatePropertyCommand.h"
#include "../SDK/APropertyDevice"
#include "../SDK/ALog"

/** \cond INTERNAL
  */

/** \class APrivatePropertyCommand
  \brief A proeprty command manages a command which changes a property from one value to another.
  */

/** Create a property command from the property device \p dev, the name of the property \p name, the
    before value \p b and the after value \p a.
  */
APrivatePropertyCommand::APrivatePropertyCommand( APropertyDevice *dev, QString name, const AVariant &b, const AVariant &a )
        : _device( dev ), _name( name ), _before( b ), _after( a )
    {
    }

void APrivatePropertyCommand::undo()
    {
    xAssert( _device );
    _device->property( _name )._value = _before;
    }

void APrivatePropertyCommand::redo()
    {
    xAssert( _device );
    _device->property( _name )._value = _after;
    }

void APrivatePropertyCommand::inform()
    {
    xAssert( _device );
    // find the property
    AProperty &ref = _device->property( _name );
    if( !ref._informing )
        {
        // inform about change
        ref._informing = TRUE;
        emit ref.onPropertyChange( &ref );
        }
    ref._informing = FALSE;
    }

int APrivatePropertyCommand::type() const
    {
    return 4;
    }

bool APrivatePropertyCommand::isEqual( APrivateCommand *comm )
    {
    // test for type equality
    if( type() == comm->type() )
        {
        // test the command has the same data as the passed command
        APrivatePropertyCommand *pComm( static_cast<APrivatePropertyCommand *>(comm) );
        if( _device == pComm->_device && _name == pComm->_name && _after.toPortable() == pComm->_after.toPortable()
            && pComm->_before.toString() == pComm->_after.toString() )
            {
            // they are equal
            return TRUE;
            }
        }
    return FALSE;
    }

bool APrivatePropertyCommand::canMerge( APrivateCommand *comm )
    {
    // can only merge same types
    if( type() == comm->type() )
        {
        // test the devices and names are the same
        APrivatePropertyCommand *pComm( static_cast<APrivatePropertyCommand *>(comm) );
        if( _device == pComm->_device && _name == pComm->_name )
            {
            return TRUE;
            }
        }
    return FALSE;
    }

void APrivatePropertyCommand::merge( APrivateCommand *comm )
    {
    APrivatePropertyCommand *pComm( static_cast<APrivatePropertyCommand *>(comm) );
    _after = pComm->_after;
    }

/** \endcond
  */
