#include "collisionArray.h"

BEGIN_PHYSICAL_NAMESPACE

void collisionArray::clear()
    {
    _collisions.clear();
    }

unsigned int collisionArray::size() const
    {
    return _collisions.size();
    }

collisionArray::collision collisionArray::operator[]( unsigned int x )
    {
    //ensure we dont go out of bounds
    jAssert( (int)x < _collisions.size() );
    return _collisions[x];
    }

const collisionArray::collision &collisionArray::operator[]( unsigned int x ) const
    {
    //ensure we dont go out of bounds
    jAssert( (int)x < _collisions.size() );
    return _collisions[x];
    }

void collisionArray::add( objectFace &a, objectFace &b, pReal r, const plane &p )
    {
    //create a collision object and append it to the collision slist
    collision temp;
    temp.faces[0] = &a;
    temp.faces[1] = &b;
    temp.restitution = r;
    temp.collisionPlane = p;

    _collisions.push_back( temp );
    }

END_PHYSICAL_NAMESPACE
