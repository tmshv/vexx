#ifndef COLLISIONARRAY_H
#define COLLISIONARRAY_H

#include "physiCalGlobal.h"
#include "shape/shape.plane.h"

BEGIN_PHYSICAL_NAMESPACE

class objectFace;

//! Holds an array of collisions, described by the involved faces and material properties
class PHYSICAL_EXPORT collisionArray
    {
public:
    //! Structure to hold a collision
    struct collision
        {
        objectFace *faces[2];
        pReal restitution;
        plane collisionPlane;
        };

    //! Empty the array
    void clear();
    //! The number of collisions in the array
    unsigned int size() const;
    //! Find the collision at index x
    collision operator[]( unsigned int x );
    //! Find the collision at index x
    const collision &operator[]( unsigned int x ) const;
    //! Add a collision to the array
    void add( objectFace &, objectFace &, pReal, const plane & );

private:
    QList <collision> _collisions;
    };

END_PHYSICAL_NAMESPACE

#endif
