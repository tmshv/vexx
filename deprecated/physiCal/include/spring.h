#ifndef MATERIALSPRING_H
#define MATERIALSPRING_H

#include "physiCalGlobal.h"
#include "particle.h"
#include "springMaterial.h"

BEGIN_PHYSICAL_NAMESPACE

//! Represents a spring in the physical system
class PHYSICAL_EXPORT spring
    {
public:
    //! holds whether this particle is internal or not
    jProperty( bool, internal );
    //! holds the primary particle for this spring
    jPtrProperty( particle, primary );
    //! holds the secondary particle for this spring
    jPtrProperty( particle, secondary );
    //! the rest length for this spring
    jProperty( pReal, restLength );
    //! the material for this spring
    jPtrProperty( const springMaterial, material );

public:
    //! create a spring between the given particles with the material m
    spring( particle &, particle &, const springMaterial &m );
    //! update this springs force ( deprecated argument )
    bool update( pReal t );

    //! divide the deformations between this spring and the given spring, for when you split a spring
    void divideDeformationsAndReset( spring & );

private:
    pReal _initialRestLength;
    springMaterial::springMode _oldMode;
    pReal _oldLength;
    };

END_PHYSICAL_NAMESPACE

#endif
