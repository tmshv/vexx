#ifndef PARTICLE_H
#define PARTICLE_H

#include "physiCalGlobal.h"

#define PHYSICAL_EULER 0x01
#define PHYSICAL_MIDPOINT 0x02
#define PHYSICAL_VERLET 0x03
#define PHYSICAL_BEEMANS 0x04

#define PHYSICAL_INTEGRATOR PHYSICAL_BEEMANS

BEGIN_PHYSICAL_NAMESPACE


//! Holds our basic particle, a velocity, acceleration and position
class PHYSICAL_EXPORT particle
    {
public:
    //! find the displacement this particle has had this step
    jProperty( vector3, displacementThisStep );
    //! find this particles inverse mass
    jProperty( pReal, inverseMass );

public:
    //! create a particle with mass m, position p, and velocity v
    particle( pReal m, position3 p=position3( 0.0 ), vector3 v=vector3( 0.0 ) );
    //! destory this particle
    virtual ~particle();

    //! set the force applied to this particle this step
    void setAppliedForce( vector3 );
    //! find the force applied force this step
    const vector3 &appliedForce( ) const;
    //! apply a force to this particle
    void applyForce( vector3 );
    //! find this particles position
    const position3 &position() const;
    //! set this particles position
    void setPosition( position3 );
    //! displace this particle by d
    void displace( vector3 d );
    //! find this particles velocity
    const vector3 &velocity() const;
    //! set this particles velocity
    void setVelocity( vector3 );
    //! apply an impulse to this particle ( velocity = velocity + in )
    void impulse( const vector3 &in );

    //! the mass of this particle
    pReal mass() const;
    //! set this particles mass
    void mass( pReal );

    //! step this partciel forward by time t
    virtual void calculate( pReal t );

    //! static value for acceleration
    static vector3 constantLinearAcceleration;
    //! static value for linear damping
    static pReal constantLinearDamping;

private:
    position3 _translate;
    vector3 _velocity;
    vector3 _appliedForce;
    position3 _oldTranslate;
    vector3 _oldVelocity;
    vector3 _oldAcceleration;
    };

END_PHYSICAL_NAMESPACE

#endif
