#ifndef ATTRIBUTECOMMAND_H
#define ATTRIBUTECOMMAND_H

#include "APrivateCommand.h"
#include "../SDK/AVariant"
class APropertyDevice;

/** \cond INTERNAL
  */

class APrivateAttributeCommand : public APrivateCommand
    {
public:
    APrivateAttributeCommand( APropertyDevice *, QString, QString, const AVariant &, const AVariant & );

    void undo();
    void redo();
    void inform();
    virtual int type() const;
    virtual void merge( APrivateCommand * );
    virtual bool canMerge( APrivateCommand * );
    virtual bool isEqual( APrivateCommand * );

private:
    APropertyDevice *_device;
    QString _property;
    QString _attribute;
    AVariant _before;
    AVariant _after;
    };

/** \endcond
  */

#endif // ATTRIBUTECOMMAND_H
