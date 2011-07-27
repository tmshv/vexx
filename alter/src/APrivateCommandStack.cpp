#include "APrivateCommandStack.h"
#include "QAction"
#include "../SDK/ALog"

/** \cond INTERNAL
  */
#define COMMAND_APPEND_MAX_TIME 75

/** \class APrivateCommandStack
  \brief A command stack allows the stacking of commands in a coherent list. It also manages stepping sensibly through the commands while keeping the integrity of the underlying data.
  */

/** The default constructor. Sets \p parent as the QObject parent.
  */
APrivateCommandStack::APrivateCommandStack(QObject *parent) : QObject(parent)
    {
    // initiate data
    _redo = _undo = 0;
    _canUndo = FALSE;
    _canRedo = FALSE;
    _index = -1;
    _changingState = FALSE;
    _currentParent = 0;
    _undoBlock = 0;
    _blockParent = 0;
    }

/** Destory the stack, deletes all data associated with the stack
  */
APrivateCommandStack::~APrivateCommandStack()
    {
    // we shouldnt be in a undo block, that would be weird
    Q_ASSERT( _undoBlock == 0 );
    // delete all commands
    foreach( APrivateCommand *c, _commands )
        {
        delete c;
        }
    }

/** Creates a QAction that can be added to menus for undoing.
  The stack keeps the action up to date and disables it a appropriate times.
  */
QAction *APrivateCommandStack::createUndoAction( QObject *parent, QString text )
    {
    // create an action and connect it to us
    _undo = new QAction( text, parent );
    connect( _undo, SIGNAL(triggered()), this, SLOT(undo()));
    updateStates();
    return _undo;
    }

/** Creates a QAction that can be added to menus for redoing.
  The stack keeps the action up to date and disables it a appropriate times.
  */
QAction *APrivateCommandStack::createRedoAction( QObject *parent, QString text )
    {
    // create an action and connect it to us
    _redo = new QAction( text, parent );
    connect( _redo, SIGNAL(triggered()), this, SLOT(redo()));
    updateStates();
    return _redo;
    }

/** Pushes the command \p comm onto the stack, and does the command
  \note There is now way to retrieve the command once it has entered the stack. The only interface with the commands is undo() and redo().
  */
void APrivateCommandStack::push( APrivateCommand *comm )
    {
    bool del = false;
    // find out if we can discard this command
    if( _commands.isEmpty() || !_commands.back()->isEqual( comm ) )
        {
        // see if we can optimise this command into the stack, or have to push it on the stack
        // if its close to another command we can assume its part of the same action
        // also if were in an undo block we can append it to the last completed command
        if( !_changingState )
            {
            if( ( ( _blockParent && _undoBlock > 0 ) || _lastPush.restart() < COMMAND_APPEND_MAX_TIME )
                && !_commands.isEmpty() )
                {
                // see if we can merge the commands
                if( _commands.back()->canMerge( comm ) )
                    {
                    _commands.back()->merge( comm );
                    // its been merged, we can delete it later
                    del = TRUE;
                    }
                else
                    {
                    // otherwise just add it as a child
                    _commands.back()->addChild( comm );
                    }
                }
            else
                {
                // erase the furute commands if were in an undone state
                if( _index+1 < _commands.size() )
                    {
                    _commands.erase( _commands.begin() + _index + 1, _commands.end() );
                    }
                // append to the stack, or merge it depending on currentParent
                if( _currentParent == 0 )
                    {
                    // set the block parent if were in a block
                    if( _undoBlock > 0 )
                        {
                        _blockParent = comm;
                        }
                    // append to stack
                    _commands << comm;
                    // set index
                    _index = _commands.size() - 1;
                    }
                else
                    {
                    // merge if we can
                    if( _commands.back()->canMerge( comm ) )
                        {
                        _commands.back()->merge( comm );
                        del = TRUE;
                        }
                    else
                        {
                        // or just add a child
                        _commands.back()->addChild( comm );
                        }
                    }
                }
            }
        else
            {
            del = TRUE;
            }
        // actually do the command
        APrivateCommand *tempOldParent = _currentParent;
        _currentParent = comm;

        comm->firstDo();


        comm->inform();

        _currentParent = tempOldParent;
        updateStates();
        }
    else
        {
        // its equal, we can delete it
        del = TRUE;
        }
    if( del )
        {
        // delete if we can
        delete comm;
        }
    }

/** Clear the command stack.
  */
void APrivateCommandStack::clear()
    {
    _index = -1;
    qDeleteAll( _commands );
    _commands.clear();
    updateStates();
    }

/** Update the states of the actions created by this object.
  \sa createUndoAction()
  \sa createRedoAction()
  */
void APrivateCommandStack::updateStates()
    {
    // if undo has been created
    if( _undo )
        {
        // set up whether we can undo
        if( _index >= 0 )
            {
            _canUndo = TRUE;
            }
        else
            {
            _canUndo = FALSE;
            }
        // set action to state
        _undo->setEnabled( _canUndo );
        }
    // if redo exists
    if( _redo )
        {
        // update action
        if( _commands.size() > (_index+1) )
            {
            _canRedo = TRUE;
            }
        else
            {
            _canRedo = FALSE;
            }
        _redo->setEnabled( _canRedo );
        }
    }

/** Step back one action in the stack.
  */
void APrivateCommandStack::undo()
    {
    bool oldCS( _changingState );

    _changingState = TRUE;
    // find if we can undo
    if( _canUndo )
        {
        _commands[_index]->fullUndo();
        }
    // inform of change
    _commands[_index]->inform();

    _changingState = oldCS;

    // update state
    _index--;
    updateStates();
    }

/** Step back one action in the stack.
  */
void APrivateCommandStack::redo()
    {
    _index++;

    bool oldCS( _changingState );
    _changingState = TRUE;

    // if we can redo
    if( _canRedo )
        {
        // complete redo
        _commands[_index]->fullRedo();
        }
    // inform and update states
    _changingState = oldCS;
    _commands[_index]->inform();

    updateStates();
    }

/** Starts a block command, all comands created after this command will become only one undo action.
  \sa endBlockChange()
  */
void APrivateCommandStack::beginBlockChange()
    {
    _undoBlock++;
    }

/** Ends a block command, all comands created after this command will become only one undo action.
  \sa beginBlockChange()
  */
void APrivateCommandStack::endBlockChange()
    {
    _undoBlock--;
    Q_ASSERT( _undoBlock >= 0 );
    if( _undoBlock == 0 )
        {
        _blockParent = 0;
        }
    }

/** \endcond
  */
