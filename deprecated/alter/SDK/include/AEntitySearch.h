#ifndef PROPERTYENTITYSEARCH_H
#define PROPERTYENTITYSEARCH_H

class AEntitySearch;
class AEntitySearchConst;

#include "AEntityArray.h"

class ALTER_EXPORT AEntitySearch : public QList <AEntity *>
    {
public:
    AEntitySearch();
    AEntitySearch( AEntityArray & );

    const AEntity &front() const;
    const AEntity &back() const;
    AEntity &front();
    AEntity &back();

    AEntity &operator[]( int );
    const AEntity &operator[]( int ) const;

    void remove( AEntity * );
    void remove( const AEntitySearch & );

    void append( AEntity * );
    void append( const AEntitySearch & );

    bool contains( AEntity * ) const;
    bool contains( AEntity & ) const;

    AEntitySearch findHasProperty( QString ) const;
    AEntitySearch findEntitys( QList <AEntityLocation> ) const;
    AEntitySearch findEntity( AEntityLocation ) const;
    AEntitySearch findDirectChildren( AEntityLocation ) const;
    AEntitySearch findChildren( AEntityLocation ) const;
    AEntitySearch findLevel( unsigned int ) const;
    AEntitySearch findType( const AEntityType & ) const;
    };

class ALTER_EXPORT AEntitySearchConst : public QList <const AEntity *>
    {
public:
    AEntitySearchConst();
    AEntitySearchConst( const AEntityArray & );

    const AEntity &operator[]( int ) const;

    const AEntity &front() const;
    const AEntity &back() const;

    void remove( const AEntity * );
    void remove( const AEntitySearchConst & );

    void append( const AEntity * );
    void append( const AEntitySearchConst & );

    bool contains( const AEntity * ) const;
    bool contains( const AEntity & ) const;

    AEntitySearchConst findHasProperty( QString ) const;
    AEntitySearchConst findEntitys( QList <AEntityLocation> ) const;
    AEntitySearchConst findEntity( AEntityLocation ) const;
    AEntitySearchConst findDirectChildren( AEntityLocation ) const;
    AEntitySearchConst findChildren( AEntityLocation ) const;
    AEntitySearchConst findLevel( unsigned int ) const;
    AEntitySearchConst findType( const AEntityType & ) const;
    };

ALTER_EXPORT AEntitySearchConst operator+( const AEntitySearchConst &, const AEntitySearchConst & );
ALTER_EXPORT AEntitySearch operator+( const AEntitySearch &, const AEntitySearch & );

ALTER_EXPORT AEntitySearchConst operator-( const AEntitySearchConst &, const AEntitySearchConst & );
ALTER_EXPORT AEntitySearch operator-( const AEntitySearch &, const AEntitySearch & );

ALTER_EXPORT AEntitySearchConst &operator+=( AEntitySearchConst &, const AEntitySearchConst & );
ALTER_EXPORT AEntitySearch &operator+=( AEntitySearch &, const AEntitySearch & );

ALTER_EXPORT AEntitySearchConst operator+( const AEntitySearchConst &, const AEntity * );
ALTER_EXPORT AEntitySearch operator+( const AEntitySearch &, AEntity * );

ALTER_EXPORT AEntitySearchConst &operator+=( AEntitySearchConst &, const AEntity * );
ALTER_EXPORT AEntitySearch &operator+=( AEntitySearch &, AEntity * );

#endif
