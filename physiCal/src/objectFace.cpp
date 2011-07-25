#include "objectFace.h"
#include "frameLog.h"

BEGIN_PHYSICAL_NAMESPACE

objectFace::objectFace( particle &a, particle &b, particle &c ) : _a( &a ), _b( &b ), _c( &c )
    {
    }

particle *objectFace::particleA()
    {
    return _a;
    }

particle *objectFace::particleB()
    {
    return _b;
    }

particle *objectFace::particleC()
    {
    return _c;
    }

void objectFace::particleA( particle *a )
    {
    _a = a;
    }

void objectFace::particleB( particle *b )
    {
    _b = b;
    }

void objectFace::particleC( particle *c )
    {
    _c = c;
    }

position3 objectFace::pointA() const
    {
    return _a->position();
    }

position3 objectFace::pointB() const
    {
    return _b->position();
    }

position3 objectFace::pointC() const
    {
    return _c->position();
    }


void objectFace::refreshBoundingCube()
    {
    // we make a bounding cube from the points in the face, plus and minus the skin width
    _bounds.clear();
    _bounds.suggest( pointA()+PHYSICAL_SKIN_WIDTH );
    _bounds.suggest( pointB()+PHYSICAL_SKIN_WIDTH );
    _bounds.suggest( pointC()+PHYSICAL_SKIN_WIDTH );
    _bounds.suggest( pointA()-PHYSICAL_SKIN_WIDTH );
    _bounds.suggest( pointB()-PHYSICAL_SKIN_WIDTH );
    _bounds.suggest( pointC()-PHYSICAL_SKIN_WIDTH );
    }

position3 objectFace::averagePosition() const
    {
    return ( pointA() + pointB() + pointC() ) / 3.0;
    }

vector3 objectFace::averageVelocity() const
    {
    // average point veclocities
    return ( _a->velocity( ) + _b->velocity( ) + _c->velocity( ) ) / 3.0;
    }

pReal objectFace::mass( ) const
    {
    // this is only used as a ratio, so add up all faces mass
    return _a->mass() + _a->mass() + _a->mass();
    }

vector3 objectFace::normal( ) const
    {
    // normalised cross product
    return vector3( pointB() - pointA() ).cross( pointC() - pointA() ).normalise();
    }

pReal objectFace::separatingVelocity( const vector3 &dir, const objectFace &a )
    {
    // assumes normalised dir
    return vector3( averageVelocity() - a.averageVelocity() ).dot( dir );
    }

void objectFace::impulse( const vector3 &in )
    {
    // apply impulse to all points
    _a->impulse( in );
    _b->impulse( in );
    _c->impulse( in );
    }

bool objectFace::collision( const objectFace &in, plane &ret ) const
    {
    // test face bounds
    if( in._bounds.test( _bounds ) )
        {
        vector3 normA = in.normal();
        vector3 normB = normal();

        // test faces face each other
        if( normA.dot( normB ) < -0.5 )
            {
            position3 avA = in.averagePosition();
            position3 avB = averagePosition();

            // vector from mid faceA to mid faceB
            vector3 aToB = avB - avA;

            // plane at tri A, and tri B, and line from midA along normal, and midB along normal
            graphics::shape::line normALine( avA, normA );
            graphics::shape::plane normBPlane( avB, normB );
            graphics::shape::line normBLine( avB, normB );
            graphics::shape::plane normAPlane( avA, normA );

            // test for collision with lineA and planeB and lineB and planeA
            pReal collA = normBPlane.collision( normALine );
            pReal collB = normAPlane.collision( normBLine );

            // if the collision is in front of the line
            if( collA < 0 || collB < 0 )
                {
                // we find an approximate radius for the triangles ( distance from points to centre added up / 4 )
                pReal radiusA = ( ( in.pointA() - avA ).length() + ( in.pointB() - avA ).length() + ( in.pointC() - avA ).length() ) / 4;
                pReal radiusB = ( ( pointA() - avB ).length() + ( pointB() - avB ).length() + ( pointC() - avB ).length() ) / 4;

                // test the collision was within the radius
                if( ( normALine.sample( collA ) - avB ).length() < radiusA ||
                    ( normBLine.sample( collB ) - avA ).length() < radiusB )
                    {
                    // find possible contributions from each face to the final normal
                    vector3 aContrib = normal();
                    vector3 bContrib = -in.normal();

                    // find the ratio to contribute from each face ( based on face mass )
                    pReal ratio = mass() / ( mass() + in.mass() );
                    if( mass() == jLib::floatLimits::infinity() )
                        {
                        ratio = 1;
                        }
                    else if( in.mass() == jLib::floatLimits::infinity() )
                        {
                        ratio = 0;
                        }

                    // create normal
                    ret.normal = ( ratio*aContrib + (1-ratio)*bContrib ).normalise();

                    // find the distance from faceA to faceB
                    pReal len = ret.normal.dot( aToB );
                    // ensure we arent returning huge wrong collisions
                    if( len > -PHYSICAL_SKIN_WIDTH )
                        {
                        // set up the collision planes position
                        ret.position = ( avB * ratio ) + ( avA * ( 1 - ratio ) );
                        return TRUE;
                        }
                    }
                }
            }
        }
    return FALSE;
    }

const boundingCube &objectFace::bounds() const
    {
    return _bounds;
    }

END_PHYSICAL_NAMESPACE
