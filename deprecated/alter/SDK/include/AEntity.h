#ifndef PROPERTYENTITY_H
#define PROPERTYENTITY_H

class AEntity;
class AProperty;
class AEntityArray;
class ASelectionEvent;

#include "AEntityLocation.h"
#include "AEntityType.h"
#include "APropertyDevice.h"
#include "QSet"

class ALTER_EXPORT AEntity : public APropertyDevice
    {
    Q_OBJECT
public:
    XProperty( AEntityArray *, array, setArray );

public:
    AEntity( );
    ~AEntity( );

    virtual AEntity *createInstance() = 0;
    AEntity *copy();

    void setName( QString );
    QString name() const;
    AEntityLocation location( ) const;

    virtual void setParent( AEntityLocation );
    AEntityLocation parent( ) const;
    virtual void selectionEvent( ASelectionEvent * );

    void hide();
    void unhide();
    bool hidden( ) const;

    virtual void childEvent();

    virtual AEntityType type() const = 0;
    virtual void relocated();
    virtual void dislocate( QHash <AEntityLocation, AEntityLocation> & );

/** \cond INTERNAL */
    void addedToArray();
/** \endcond */

protected:
    virtual void prePropertyChanged( AProperty * );

private:
    void validateName( );
    bool parentTreeIsValid();
    AEntityLocation _oldLocation;
    QString _oldName;
    };

typedef AEntity *(*AEntityCreatorFn)( );

#define A_ENTITY( clName, orgName, entName )  \
    defineClassType( orgName, entName )         \
    static AEntity *create()                    \
        {                                       \
        return new clName();                    \
        }                                       \
    virtual AEntity *createInstance()           \
        {                                       \
        return new clName();                    \
        }                                       \

#endif
