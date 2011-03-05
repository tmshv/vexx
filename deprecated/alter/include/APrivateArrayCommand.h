#ifndef ARRAYCOMMAND_H
#define ARRAYCOMMAND_H

#include "../SDK/AEntity"
#include "../SDK/AEntityArray"
#include "APrivateCommand.h"

/** \cond INTERNAL
  */

class APrivateArrayCommand : public APrivateCommand
    {
public:
    enum commandMode { Add, Remove };
    APrivateArrayCommand( AEntityArray *, AEntity *, commandMode );
    ~APrivateArrayCommand( );

    virtual void firstDo();
    virtual void undo();
    virtual void redo();
    virtual void inform();
    virtual int type() const;

private:
    void add();
    void remove();
    AEntityArray *_array;
    AEntity *_ent;
    bool _owner;
    };

/** \endcond
  */
#endif
