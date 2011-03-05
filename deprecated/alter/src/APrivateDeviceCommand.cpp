#include "APrivateDeviceCommand.h"
#include "../SDK/ALog"

/** \cond INTERNAL
  */

/** \class APrivateDeviceCommand
  \brief The command manages adding and removing of properties
  */

/** Create a command for the entity \p ent, the mode \p mode and the property \p data
  */
APrivateDeviceCommand::APrivateDeviceCommand( APropertyDevice *ent, QString name, commandMode mode, AProperty *data ) : _ent( ent ),
        _propertyName( name ), _propertyValue( data )
    {
    // setup ownership of the property
    if( mode == Add )
        {
        _owner = TRUE;
        }
    else
        {
        _owner = FALSE;
        }
    }

/** Delete the command, and if the command owns the data that too.
  */
APrivateDeviceCommand::~APrivateDeviceCommand()
    {
    if( _owner )
        {
        delete _propertyValue;
        }
    }

void APrivateDeviceCommand::undo()
    {
    // add or remove depending on current ownership
    if( _owner == TRUE )
        {
        add();
        }
    else
        {
        remove();
        }
    }

void APrivateDeviceCommand::redo()
    {
    // add or remove depending on current ownership
    if( _owner == TRUE )
        {
        add();
        }
    else
        {
        remove();
        }
    }

void APrivateDeviceCommand::inform()
    {
    Q_EMIT _ent->propertiesChanged();
    }

/** Add the property to the device
  */
void APrivateDeviceCommand::add()
    {
    // add to entity, then connect up signals
    _ent->_properties.insert( _propertyName, _propertyValue );
    _propertyValue->connect( _propertyValue, SIGNAL(onCommand(APrivateCommand**)), _ent, SIGNAL(onCommand(APrivateCommand**)) );
    _propertyValue->connect( _propertyValue, SIGNAL(onPropertyChange(AProperty*)), _ent, SLOT(prePropertyChanged(AProperty*)) );
    _owner = FALSE;
    }

/** Remove the property from the device
  */
void APrivateDeviceCommand::remove()
    {
    // disconnect signals, then remove from entity
    _propertyValue->disconnect( _propertyValue, SIGNAL(onCommand(APrivateCommand**)), _ent, SIGNAL(onCommand(APrivateCommand**)) );
    _propertyValue->disconnect( _propertyValue, SIGNAL(onPropertyChange(AProperty*)), _ent, SLOT(prePropertyChanged(AProperty*)) );
    _ent->_properties.remove( _propertyName );
    _owner = TRUE;
    }


int APrivateDeviceCommand::type() const
    {
    return 3;
    }

/** \endcond
  */
