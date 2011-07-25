#include "particle.h"
#include "frameLog.h"

BEGIN_PHYSICAL_NAMESPACE


// setup static parameters
vector3 particle::constantLinearAcceleration = vector3( 0, -9.81, 0 );
// this is a lot at 10000 fps, when applied every frame
pReal particle::constantLinearDamping = 0.9999;

particle::particle( pReal mass, position3 position, vector3 velocity ) : _displacementThisStep( 0.0 ), _appliedForce( 0.0 ), _oldAcceleration( 0.0 )
    {
    _inverseMass = 1 / mass;
    _oldTranslate = _translate = position;
    _oldVelocity = _velocity = velocity;
    }

particle::~particle()
    {
    }

void particle::setAppliedForce( vector3 in )
    {
    _appliedForce = in;
    }

const vector3 &particle::appliedForce( ) const
    {
    return _appliedForce;
    }

void particle::applyForce( vector3 force )
    {
    _appliedForce += force;
    }

void particle::calculate( pReal time )
    {
    if( _inverseMass == 0 )
        {
        return;
        }

#if PHYSICAL_INTEGRATOR == PHYSICAL_BEEMANS
    // BEEMANS integrator
    _appliedForce *= _inverseMass;
    _appliedForce += constantLinearAcceleration;
    _translate += (_velocity*time) + (2.0/3.0)*_appliedForce*time*time - (1.0/6.0)*_oldAcceleration*time*time;
    _velocity += (1.0/3.0)*_appliedForce*time + (5.0/6.0)*_appliedForce*time - (1.0/6.0)*_oldAcceleration*time;
#else
# if PHYSICAL_INTEGRATOR == PHYSICAL_VERLET
    // VERLET integrator
    _appliedForce *= _inverseMass;
    _appliedForce += constantLinearAcceleration;

    _translate += _velocity * time + 0.5 * _appliedForce * time * time;

    _velocity += 0.5 * ( _oldAcceleration + _appliedForce ) * time;
    _velocity *= constantLinearDamping;

# else
    // semi-implicit EULER Integrator
    _velocity += time * ( _appliedForce * _inverseMass + constantLinearAcceleration );

    _velocity *= constantLinearDamping;

    _translate += time * _velocity + 0.5 * ( _appliedForce * time * time );
# endif
#endif

    // test fro explode and print appologyu if it has! ( good debugging stop point )
    if( _translate.length() == jLib::floatLimits::infinity() || _translate.length() == jLib::floatLimits::nan() )
        {
        frameLog( "Particle "<<this<<" has exploded applied force was "<<_appliedForce<<" velocity was "<<_velocity );
        }

    // setup old values for use in next iteration with beemans and verlet
    _oldVelocity = _velocity;
    _oldTranslate = _translate;
    _oldAcceleration = _appliedForce;
    _appliedForce = 0;
    _displacementThisStep = 0.0;
    }

const position3 &particle::position() const
    {
    return _translate;
    }

void particle::setPosition( position3 in )
    {
    _translate = in;
    }

void particle::displace( vector3 disp )
    {
    _translate += disp;
    }

const vector3 &particle::velocity() const
    {
    return _velocity;
    }

void particle::setVelocity( vector3 in )
    {
    _velocity = in;
    }

void particle::impulse( const vector3 &in )
    {
    _velocity += in;
    }

pReal particle::mass() const
    {
    return 1 / _inverseMass;
    }

void particle::mass( pReal ma )
    {
    _inverseMass = 1 / ma;
    }

END_PHYSICAL_NAMESPACE
