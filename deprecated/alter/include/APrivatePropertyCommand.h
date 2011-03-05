#ifndef PROPERTYCOMMAND_H
#define PROPERTYCOMMAND_H

#include "APrivateCommand.h"
#include "../SDK/AVariant"
#include "../SDK/AProperty"
#include "../SDK/AEntity"

/** \cond INTERNAL
  */

class APrivatePropertyCommand : public APrivateCommand
    {
public:
    APrivatePropertyCommand( APropertyDevice *, QString, const AVariant &, const AVariant & );

    void undo();
    void redo();
    virtual void inform();
    virtual int type() const;
    virtual void merge( APrivateCommand * );
    virtual bool canMerge( APrivateCommand * );
    virtual bool isEqual( APrivateCommand * );

private:
    APropertyDevice *_device;
    QString _name;
    AVariant _before;
    AVariant _after;
    };

/** \endcond
  */
#endif // PROPERTYCOMMAND_H
