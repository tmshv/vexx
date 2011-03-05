#ifndef SHAPE_CUBOID_H
#define SHAPE_CUBOID_H

#include "shape/shape.types.h"

BEGIN_PHYSICAL_NAMESPACE

class PHYSICAL_EXPORT cuboid
    {
    jSpecificProperty( pVector, minimum, protected, public )
    jSpecificProperty( pVector, maximum, protected, public )
public:
    cuboid();
    cuboid( pVector, pVector );

    pVector size() const;
    pReal volume() const;
    void clear();
    void suggest( const pVector & );
    void suggest( const cuboid & );
    bool test( const cuboid & ) const;
    bool test( const pVector & ) const;

    static cuboid fromCuboid( const cuboid &, pVector, pVector );

private:
    bool _clear;
    };

END_PHYSICAL_NAMESPACE

#endif
