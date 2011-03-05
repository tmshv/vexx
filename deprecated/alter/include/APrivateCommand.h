#ifndef COMMAND_H
#define COMMAND_H

#include "../SDK/ALog"
#include "../SDK/AGlobal"

#include "typeinfo"

/** \cond INTERNAL
  */

/** \class APrivateCommand
  \brief \c APrivateCommand is the abstract root command of the commandStack. other commands inherit from this one to implement functionality such as array additions and property changes.
  */
class APrivateCommand
    {
public:
    /** Destrosy the command and all its children
      */
    virtual ~APrivateCommand()
        {
        // delete children
        foreach( APrivateCommand *c, _commands )
            {
            delete c;
            }
        }
    /** Called the first time the command is executed
       \note Default implementation calls \p redo()
       */
    virtual void firstDo()
        {
        redo();
        }

    /** Fully undoes this command, including all its children, in the correct order.
      */
    void fullUndo()
        {
        // undo children in order
        for( int x=(_commands.size()-1); x>=0; x-- )
            {
            _commands[x]->fullUndo();
            }
        //undo self
        undo();
        }

    /** Fully redoes all this commands children and itself, in the correct order.
      */
    void fullRedo()
        {
        // redo self
        redo();
        // undo children
        foreach( APrivateCommand *c, _commands )
            {
            c->fullRedo();
            }
        }

    /** Returns if this command is equal to the given command. If commands are equal and next to each other in the stack, we can discard one.
      \note the default implementation returns false.
      */
    virtual bool isEqual( APrivateCommand * ) { return false; }

    /** Implement this function to inform the array of changes made by this command
      */
    virtual void inform() = 0;

    /** Add a child to the list of commands this command manages. This command takes ownership of the passed command.
      */
    void addChild( APrivateCommand *comm )
        {
        _commands << comm;
        }

    /** Returns whether this command can merge with the given command.
      \note Default implementation returns false.
      \note Child classes should also implement merge() with this function.
      \sa merge()
      */
    virtual bool canMerge( APrivateCommand * ) { return false; }

    /** Merge this command with the given command
      */
    virtual void merge( APrivateCommand * ) { }

    /** Returns an int unique to this command type.
      */
    virtual int type() const = 0;

protected:
    /** Undo the command
      */
    virtual void undo() = 0;

    /** Redo the command.
      */
    virtual void redo() = 0;

private:
    QList <APrivateCommand *> _commands;
    };

/** \endcond
  */
#endif // COMMAND_H
