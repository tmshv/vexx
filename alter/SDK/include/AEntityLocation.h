#ifndef ENTITYLOCATION_H
#define ENTITYLOCATION_H

#include "AGlobal.h"
#include "../SDK/AVariant"

class ALTER_EXPORT AEntityLocation
    {
    XROProperty( QString, path );
public:
    AEntityLocation( const char * );
    AEntityLocation( AVariant );
    AEntityLocation( QString="/" );
    AEntityLocation( AEntityLocation, QString );
    void path( QString );
    void path( AEntityLocation, QString );
    void directory( QString );
    QString directory( ) const;
    AEntityLocation appendLocation( const AEntityLocation & ) const;
    AEntityLocation subtractBasePath( const AEntityLocation & ) const;
    AEntityLocation parent( ) const;
    void name( QString );
    QString name( ) const;
    AEntityLocation upToLevel( int ) const;
    unsigned int level( ) const;
    QStringList levels( ) const;
    bool isDirectChild( AEntityLocation ) const;
    bool isChild( AEntityLocation ) const;
    bool isRoot( ) const;
    operator AVariant() const;
    bool operator==( QString & ) const;
    bool operator==( const AEntityLocation & ) const;
    bool operator!=( const AEntityLocation &in ) const
        { return !( *this == in ); }
    };

ALTER_EXPORT AEntityLocation operator+( const AEntityLocation &, const AEntityLocation & );
ALTER_EXPORT bool operator==( const QString &, const AEntityLocation & );
ALTER_EXPORT uint qHash( AEntityLocation );
#endif
