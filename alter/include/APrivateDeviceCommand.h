#ifndef DEVICECOMMAND_H
#define DEVICECOMMAND_H

#include "../SDK/APropertyDevice"
#include "APrivateCommand.h"

/** \cond INTERNAL
  */

class APrivateDeviceCommand : public APrivateCommand
    {
public:
    enum commandMode { Add, Remove };
    APrivateDeviceCommand( APropertyDevice *, QString, commandMode, AProperty * = 0 );
    ~APrivateDeviceCommand();

    virtual void undo();
    virtual void redo();
    virtual void inform();
    virtual int type() const;
private:
    void add();
    void remove();
    APropertyDevice *_ent;
    QString _propertyName;
    AProperty *_propertyValue;
    bool _owner;
    };

/** \endcond
  */

#endif // DEVICECOMMAND_H
