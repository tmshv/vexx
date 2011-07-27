#ifndef PROPERTYMANAGER_H
#define PROPERTYMANAGER_H

#include "QList"
#include "AGlobal.h"

class AEntity;
class AProperty;
class QString;
class QWidget;

class ALTER_EXPORT APropertyManager
    {
public:
    APropertyManager();
    enum propertyMode { Compact, Detailed };

    void setMode( propertyMode );

    void addEntity( AEntity * );
    void addProperty( AProperty & );
    QWidget *createWidget( ) const;

    static QWidget *createWidget( AEntity * );
    static QWidget *createWidget( AProperty & );
    static QWidget *createWidget( AProperty &, int forceType );

private:
    propertyMode _mode;
    QList <AEntity *> _devices;
    QList <AProperty *> _properties;
    };

#endif // PROPERTYMANAGER_H
