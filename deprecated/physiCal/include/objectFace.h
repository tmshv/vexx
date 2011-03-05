#ifndef OBJECTFACE_H
#define OBJECTFACE_H

#include "physiCalGlobal.h"
#include "particle.h"
#include "boundingCube.h"
#include "shape/shape.plane.h"

BEGIN_PHYSICAL_NAMESPACE

#define PHYSICAL_SKIN_WIDTH 0.4

//! Represents a face of an object in the world
class objectFace
    {
public:
    //! create a face from these particles
    objectFace( particle &, particle &, particle & );

    //! find the position of particleA
    position3 pointA() const;
    //! find the position of particleB
    position3 pointB() const;
    //! find the position of particleB
    position3 pointC() const;

    //! find the particle for pointA on this triangle
    particle *particleA();
    //! find the particle for pointB on this triangle
    particle *particleB();
    //! find the particle for pointC on this triangle
    particle *particleC();

    //! Set particleA
    void particleA( particle * );
    //! Set particleB
    void particleB( particle * );
    //! Set particleC
    void particleC( particle * );

    //! Refresh the bounding cube for this face
    void refreshBoundingCube();

    //! Find the mid position on this face
    position3 averagePosition() const;
    //! find the average velocity of this face
    position3 averageVelocity() const;
    //! find the mass of this face ( mass of particles added up )
    pReal mass() const;
    //! Return the normal of this face
    vector3 normal() const;
    //! the separating velocity of this face and the given face, along the vector dir ( dir should be normalised )
    pReal separatingVelocity( const vector3 &dir, const objectFace &other );

    //! impulse this face by the given vector
    void impulse( const vector3 &vec );

    //! find collisions between this face and the given face, returns true if collided, and plane is the collision plane
    bool collision( const objectFace &, plane & ) const;

    //! return the bounding cube of this plane
    const boundingCube &bounds() const;

private:
    boundingCube _bounds;
    particle *_a;
    particle *_b;
    particle *_c;
    };

END_PHYSICAL_NAMESPACE

#endif // OBJECTFACE_H
