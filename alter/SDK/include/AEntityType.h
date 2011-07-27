#ifndef PROPERTYENTITYTYPE_H
#define PROPERTYENTITYTYPE_H

#include "AGlobal.h"

class ALTER_EXPORT AEntityType
    {
public:
    XProperty( QString, name, setName );
    XProperty( QString, group, setGroup );
    XROProperty( QByteArray, hash );

public:
    AEntityType( QString o, QString n );
    AEntityType( QVariant );

    operator QVariant();
    bool operator ==( const AEntityType & ) const;
    bool operator !=( const AEntityType & ) const;
    };

inline uint qHash( const AEntityType &in )
    {
    return qHash( in.group() + in.name() );
    }

#define defineClassType( owner, name )              \
public:                                             \
    virtual AEntityType type() const                \
        {                                           \
        return AEntityType( owner, name );          \
        }                                           \
    static AEntityType identifier()                 \
        {                                           \
        return AEntityType( owner, name );          \
        }


#endif
