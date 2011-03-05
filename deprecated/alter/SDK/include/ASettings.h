#ifndef ASETTINGS_H
#define ASETTINGS_H

#include "AGlobal.h"
#include "AVariant.h"

class APlugin;
class AEntity;
class AEntityArray;

class ALTER_EXPORT ASettings
    {
public:
    ASettings( APlugin *, QString );
    ASettings( const APlugin *, QString );
    ASettings( AEntityArray *, QString );
    ASettings( const AEntityArray *, QString );

    AVariant ensureValue( QString, AVariant );
    AVariant value( QString, AVariant=AVariant() ) const;
    void setValue( QString, AVariant );

private:
    AEntity *_entity;
    const AEntity *_constEntity;
    };

#endif
