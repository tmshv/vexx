#ifndef MATERIAL_H
#define MATERIAL_H

#include "physiCalGlobal.h"

BEGIN_PHYSICAL_NAMESPACE

//! Holds a material for a spring, and helps springs deform
class PHYSICAL_EXPORT springMaterial
    {
public:
    //! this springs spring constant
    jProperty( pReal, springConstant );
    //! this springs spring damping
    jProperty( pReal, springDamping );
    //! this springs elastic limit
    jProperty( pReal, elasticLimit );
    //! this springs ultimate tensile stress
    jProperty( pReal, ultimateTensileStress );
    //! this springs restitution
    jProperty( pReal, restitution );
    //! this springs density
    jProperty( pReal, density );

public:
    //! enum describing the mode a spring is in
    enum springMode { Loading, UnLoading, Rest };
    //! create a spring material from the values given
    springMaterial( pReal density,
                    pReal restitution = 1,
                    pReal springConst = 1,
                    pReal springDamping = 0,
                    pReal elasticLimit = jLib::floatLimits::infinity(),
                    pReal ultimateTensileStress = jLib::floatLimits::infinity() );
    //! find the spring constant for a spring with extension e in mode m
    pReal constant( pReal e, springMode m ) const;
    //! calculate a new length for this spring with current length l, and rest length r
    pReal calculatePlasticLength( pReal l, pReal r ) const;

    //! create an infinite material ( ie doesnt move ever )
    static springMaterial infinite( );
    };

END_PHYSICAL_NAMESPACE

#endif
