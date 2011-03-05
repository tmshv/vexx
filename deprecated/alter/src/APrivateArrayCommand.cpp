#include "APrivateArrayCommand.h"
#include "../SDK/ALog"

/** \cond INTERNAL
  */

/** \class APrivateArrayCommand
  \brief The \c arrayCommand class is used to represent an array operation. Either an addition of an entity to the array, or a removal.
  */

/** Create an array command, from the array \p arr, entity \p ent, and the mode of the command \p mode.
  */
APrivateArrayCommand::APrivateArrayCommand( AEntityArray *arr, AEntity *ent, commandMode mode ) : _array( arr ), _ent( ent )
    {
    // decide whether we own the entitiy given
    if( mode == Add )
        {
        _owner = TRUE;
        }
    if( mode == Remove )
        {
        _owner = FALSE;
        }
    }

APrivateArrayCommand::~APrivateArrayCommand( )
    {
    // delete the entity if we own it
    if( _owner )
        {
        delete _ent;
        }
    }

int APrivateArrayCommand::type() const
    {
    return 1;
    }

void APrivateArrayCommand::undo()
    {
    // operate based on whether we own the entity or not
    if( _owner )
        {
        add();
        }
    else
        {
        remove();
        }
    }

void APrivateArrayCommand::redo()
    {
    // operate based on whether we own the entity or not
    if( _owner )
        {
        add();
        }
    else
        {
        remove();
        }
    }

void APrivateArrayCommand::firstDo()
    {
    // do and add to the array
    redo();
    if( !_owner )
        {
        _ent->addedToArray();
        }
    }

void APrivateArrayCommand::inform()
    {
    // emit a correct signal about the operation
    if( _owner )
        {
        Q_EMIT _array->entityRemoved( _ent );
        _ent->setArray( 0 );
        }
    else
        {
        Q_EMIT _array->entityAdded( _ent );
        }
    }

/** This function is called by \c undo() and \c redo() to add the entity to the array
  */
void APrivateArrayCommand::add()
    {
    // check the entity isnt in the array already
    foreach( AEntity *e, _array->_entities )
        {
        if( e == _ent )
            {
            return;
            }
        }

    // connect it to signals
    _ent->connect( _ent, SIGNAL(onCommand(APrivateCommand**)), _array, SIGNAL(onCommand(APrivateCommand**)) );
    // add it
    _array->_entities.push_back( _ent );
    // set its array pointer
    _ent->setArray( _array );
    // we no longer own it
    _owner = FALSE;
    }

/** This function is called by \c undo() and \c redo() to remove the entity from the array
  */
void APrivateArrayCommand::remove()
    {
    // remove it from the array
    for( int x=0; x<_array->_entities.size(); x++ )
        {
        if( _array->_entities[x] == _ent )
            {
            _array->_entities.erase( _array->_entities.begin() + x );
            }
        }
    // disconnect from the array
    _ent->disconnect( _ent, SIGNAL(onCommand(APrivateCommand**)), _array, SIGNAL(onCommand(APrivateCommand**)) );
    // we now own it
    _owner = TRUE;
    }

/** \endcond
  */
