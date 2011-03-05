#include "APrivateAttributeCommand.h"
#include "../SDK/APropertyDevice"
#include "../SDK/AVariant"

/** \cond INTERNAL
  */
/** \class APrivateAttributeCommand
  \brief Represents a change to an attribute.
  */

/** Create an attribute command, from the device \p dev, property \p prop, attribute \p attrib and \p before and \p after values
  */
APrivateAttributeCommand::APrivateAttributeCommand( APropertyDevice *dev, QString prop, QString attrib, const AVariant &before, const AVariant &after )
        : _device( dev ), _property( prop ), _attribute( attrib ), _before( before ), _after( after )
    {
    }

void APrivateAttributeCommand::undo()
    {
    if( _before.type() != AVariant::Null )
        {
        // set the property
        _device->property( _property )._attributes[_attribute] = _before;
        }
    else
        {
        _device->property( _property )._attributes.remove( _attribute );
        }
    }

void APrivateAttributeCommand::redo()
    {
    if( _after.type() != AVariant::Null )
        {
        // set the property
        _device->property( _property )._attributes[_attribute] = _after;
        }
    else
        {
        _device->property( _property )._attributes.remove( _attribute );
        }
    }

void APrivateAttributeCommand::inform()
    {
    xAssert( _device );
    // get a property reference
    AProperty &ref = _device->property( _property );
    // check we dont loop round
    if( !ref._informing )
        {
        // inform
        ref._informing = TRUE;
        emit ref.onPropertyChange( &ref );
        }
    // finished informing
    ref._informing = FALSE;
    }

int APrivateAttributeCommand::type() const
    {
    return 2;
    }

bool APrivateAttributeCommand::isEqual( APrivateCommand *comm )
    {
    if( type() == comm->type() )
        {
        // test if the values are equal by comparing their internal data
        APrivateAttributeCommand *pComm( static_cast<APrivateAttributeCommand *>(comm) );
        if( _device == pComm->_device && _property == pComm->_property &&
            _attribute == pComm->_attribute && _after.toString() == pComm->_after.toString() )
            {
            return TRUE;
            }
        }
    return FALSE;
    }

bool APrivateAttributeCommand::canMerge( APrivateCommand *comm )
    {
    // need the same type to merge
    if( type() == comm->type() )
        {
        // and the same underlying devices and properties
        APrivateAttributeCommand *pComm( static_cast<APrivateAttributeCommand *>(comm) );
        if( _device == pComm->_device && _property == pComm->_property && _attribute == pComm->_attribute )
            {
            return TRUE;
            }
        }
    return FALSE;
    }

void APrivateAttributeCommand::merge( APrivateCommand *comm )
    {
    // take its after value and make it mine
    APrivateAttributeCommand *pComm( static_cast<APrivateAttributeCommand *>(comm) );
    _after = pComm->_after;
    }

/** \endcond
  */
