#include "spring.h"
#include "frameLog.h"
#include "QDebug"

BEGIN_PHYSICAL_NAMESPACE

spring::spring( particle &inA, particle &inB, const springMaterial &mat ) : _primary( &inA ), _secondary( &inB ),
        _restLength( ( primary().position() - secondary().position() ).length() ), _material( &mat )
    {
    _oldLength = _initialRestLength = _restLength;
    _oldMode = springMaterial::Rest;
    _internal = FALSE;
    }

bool spring::update( pReal )
    {
    // vector between two springs
    vector3 force = ( secondary().position() - primary().position() );
    // distance between particles
    pReal length( force.length() );
    // magnitude of force, equal to displacement from rest length * spring constant
    pReal magnitude = ( length - restLength() ) * material().constant( force.length() / _initialRestLength, springMaterial::Loading );

    // normal of force direction
    vector3 forceNorm = force.normalise();

    // the actual force to exert on both particles ( negated one way )
    vector3 exert = magnitude * forceNorm - material().springDamping() * ( primary().velocity() - secondary().velocity() );

    // adjust the rest legnth if we are plastically deforming
    if( !jLib::math::fcmp( _oldLength, length ) && fabs( _oldLength - restLength() ) > fabs( length - restLength() ) )
        {
        restLength( material().calculatePlasticLength( length, restLength() ) );
        }

    // apply the forces
    secondary().applyForce( exert.negate() );
    primary().applyForce( exert );
    _oldLength = length;

    // tear if over the plastic limit
    if( fabs( length - restLength() ) > material().ultimateTensileStress() )
        {
        return TRUE;
        }
    return FALSE;
    }

void spring::divideDeformationsAndReset( spring &in )
    {
    // find current spring lengths
    pReal aLen( ( secondary().position() - primary().position() ).length() );
    pReal bLen( ( in.secondary().position() - in.primary().position() ).length() );
    // find the total length
    pReal totalLength( aLen + bLen );

    //Initial Rest Length
        {
        // find the length we are aiming at
        pReal aimLength( _initialRestLength );
        pReal totalLength( aLen + bLen );

        // divide the length between them
        _initialRestLength = ( aimLength / totalLength ) * aLen;
        in._initialRestLength = ( aimLength / totalLength ) * bLen;
        }
    //Rest Length
        {
        pReal aimLength( restLength() );

        // divide the length between them
        restLength( ( aimLength / totalLength ) * aLen );
        in.restLength( ( aimLength / totalLength ) * bLen );
        }
    }

END_PHYSICAL_NAMESPACE
