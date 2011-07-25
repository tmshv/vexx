#include "object.h"
#include "frameLog.h"
#include "shape/shape.line.h"
#include "shape/shape.triangle.h"
#include "shape/shape.sphere.h"

#include "QDebug"

BEGIN_PHYSICAL_NAMESPACE

#define ASSERT_MESH_IS_VALID( mesh ) mesh->testFaceSpringLinks()

// find the area of a polygon, ie a list of points in space, N indicates the normal of the polygon sign indicates winding.
//Taken and adapted from http://softsurfer.com/Archive/algorithm_0101/algorithm_0101.htm#3D%20Polygons
pReal areaPolygon( const QList <pVector> &V, vector3 N )
    {
    float area = 0;
    float an, ax, ay, az;  // abs value of normal and its coords
    int coord;           // coord to ignore: 1=x, 2=y, 3=z

    // select largest abs coordinate to ignore for projection
    ax = ( N.x()>0 ? N.x() : -N.x() );     // abs x-coord
    ay = ( N.y()>0 ? N.y() : -N.y() );     // abs y-coord
    az = ( N.z()>0 ? N.z() : -N.z() );     // abs z-coord

    coord = 3;                     // ignore z-coord
    if (ax > ay) {
        if (ax > az) coord = 1;    // ignore x-coord
    }
    else if (ay > az) coord = 2;   // ignore y-coord

    // compute area of the 2D projection
    for ( int k=0; k<V.size(); k++ )
        {
        int i( (k+1) % V.size() ), j( (k+2) % V.size() );
        switch( coord )
            {
            case 1:
                area += ( V[i].y() * (V[j].z() - V[k].z() ) );
                break;
            case 2:
                area += ( V[i].x() * (V[j].z() - V[k].z() ) );
                break;
            case 3:
                area += ( V[i].x() * (V[j].y() - V[k].y() ) );
                break;
            }
        }

    // scale to get area before projection
    an = sqrt( ax*ax + ay*ay + az*az);  // length of normal vector
    switch (coord)
        {
        case 1:
            area *= (an / (2*ax));
            break;
        case 2:
            area *= (an / (2*ay));
            break;
        case 3:
            area *= (an / (2*az));
        }
    return area;
    }

// find double the area of a triangle of particles
pReal doubleTriangleArea( particle *a, particle *b, particle *c )
    {
    // find cross product of two edges and return
    return ( b->position() - a->position() ).cross( c->position() - a->position() ).length();
    }

// internal edge class used for ripping patterns
struct edge
    {
    enum edgeMode { Normal, FaceEntry, Internal };
    edge( objectFace *_face, spring *_thisSpring, pReal _t ) : mode( Normal ), face( _face ), thisSpring( _thisSpring ), t( _t ) { }
    edge( objectFace *_face, pVector _p ) : mode( FaceEntry ), face( _face ), thisSpring( 0 ), t( 0 ), pos( _p ) { }
    edge( pVector _p ) : mode( Internal ), face( 0 ), thisSpring( 0 ), t( 0 ), pos( _p ) { }
    edgeMode mode;
    objectFace *face;
    spring *thisSpring;
    pReal t;
    pVector pos;
    };

object::object( const springMaterial &mat, const jLib::graphics::primitives::polygonMesh &arr, position3 pos, vector3 vel, unsigned int ePPV, unsigned int sPP, float maxlength )
    {
    // set material
    material(mat);

    // for each point
    for( unsigned int x=0; x<arr.points.size(); x++ )
        {
        // store transformed point
        position3 pointPos( pos + arr.points.get(x) );
        // append to array
        _points.push_back( particle( 1, pointPos, vel ) );
        // expand bounds to fit
        _bounds.suggest( pointPos );
        }



    jLib::list <jLib::graphics::shape::triangle> tris;
    // Add Faces
        {
        for( unsigned int x=0; x<arr.sizePolygonArray(); x++ )
            {
            // simply decompose polygons to triangles. wont work for advanced polygons. but we arent expecting anything more than quads
            for( unsigned int y=1; y<(arr.sizePolygon( x )-1); y++ )
                {
                tris.push_back( jLib::graphics::shape::triangle( pos+arr.points.get(arr.getPolygonVertexPoint( x, 0 )),
                                                              pos+arr.points.get(arr.getPolygonVertexPoint( x, y )),
                                                              pos+arr.points.get(arr.getPolygonVertexPoint( x, y+1 ))));
                }
            }
        }

    // find the planes of each triange for collision
    jLib::list <jLib::graphics::shape::plane> planes;
    // Add Faces
        {
        FOREACH( tris, t )
            {
            planes.push_back( jLib::graphics::shape::plane( t->pointA,
                            pVector( t->pointB - t->pointA ).cross( pVector( t->pointC - t->pointA ).normalise() ) ) );
            }
        }

    // find extra points needed
    unsigned int extraPoints = ePPV * (unsigned int)(ceil(_bounds.volume())+0.5);

    // add extra points
    for( unsigned int x=0; x<extraPoints; x++ )
        {
        // pick a random position within bounds
        position3 randPos( _bounds.minimum() + position3( jLib::math::random::getF( 0, _bounds.size().x() ),
                                                          jLib::math::random::getF( 0, _bounds.size().y() ),
                                                          jLib::math::random::getF( 0, _bounds.size().z() ) ) );
        unsigned int collisions = 0;
        // create a line which goes to somewherer we know is outside
        jLib::graphics::shape::line toOutside( randPos, _bounds.size() );

        // this algorithm tests if a point is inside a mesh...

        // foreach plane
        jLib::list <jLib::graphics::shape::plane>::iterator p = planes.begin();
        // and triangle
        FOREACHCONST( tris, t )
            {
            // test for collision with this plane
            pReal smpl = p->collision( toOutside );

            // if collided
            if( smpl > 0 && t->liesOn( toOutside.sample( smpl ) ) )
                {
                // we collided add one to counter
                collisions++;
                }
            // increment plane iterator too.
            p++;
            }

        // if collisions was odd we're inside, otherwise outside.
        if( (collisions&1) == 1 )
            {
            // if were inside add to the point list.
            _points.push_back( particle( 1, randPos, vel ) );
            }
        }

    //Insert Springs
        {
        jLib::list <jLib::pair<unsigned int, unsigned int> > edges = arr.listTriangleEdges();
        // store the last external edge indes, all the others are random internal springs
        unsigned int finalExternalIndex( edges.size() - 1 );

        // Add Random springs only if there are points in the array
        if( _points.size() > 0 )
            {
            // and we have been asked to add more than 0 points per spring
            if( sPP > 0 )
                {
                // foreach point
                for( int p=0; p<_points.size(); p++ )
                    {
                    unsigned int num = 0;
                    // were going to try to connect to the number of points in the array, if really random we ensure we almost tried all points for connection
                    for( int x=0; x<_points.size(); x++ )
                        {
                        // pick a random point
                        int randPt = jLib::math::random::getI( 0, ( _points.size() - 1 ) );
                        // not ourselves, and if its not over maxLength for internal springs
                        if( randPt != p && (_points[randPt].position()-_points[p].position()).length() < maxlength )
                            {
                            // create the line
                            jLib::graphics::shape::line line( _points[randPt].position(), _points[p].position()-_points[randPt].position() );
                            unsigned int collNum=0;
                            // create a line from the middle of the line, and test it is not outside the mesh, ensuring most lines are completely internal
                            jLib::graphics::shape::line midLine( line.sample( 0.5 ), vector3( 1, 1.2, 1 ) );

                            // foreach plane iterator
                            jLib::list <jLib::graphics::shape::plane>::iterator plan = planes.begin();
                            // foreach triangle aswell
                            FOREACHCONST( tris, t )
                                {
                                // find the collision
                                pReal smpl = plan->collision( midLine );
                                if( smpl > 0 && t->liesOn( midLine.sample( smpl ) ) )
                                    {
                                    // if collided, record
                                    collNum++;
                                    }
                                // inc plane iterator
                                plan++;
                                }

                            // if collisions is odd, and we successfully insert
                            if( (collNum&1) == 1 && jLib::graphics::primitives::insertIfUnique( p, randPt, edges ) )
                                {
                                // inc numver of insereted
                                num++;
                                }
                            // if the number of inserted is >= to the number requested
                            if( num >= sPP )
                                {
                                break;
                                }
                            }
                        }
                    }
                }
            }
        // foreach edge we want to insert
        jLib::list <jLib::pair<unsigned int, unsigned int> >::iterator it = edges.begin();
        for( unsigned int x=0; x<edges.size(); x++)
            {
            // add the springs.
            addSpring( &_points[it->first], &_points[it->second], mat, x > finalExternalIndex );
            it++;
            }
        }

    // evenly distribute the mass between all particles
    redistributeMass();

    // add the faces fromt the mesh provided to the system
    for( unsigned int x=0; x<arr.sizePolygonArray(); x++ )
        {
        for( unsigned int y=1; y<(arr.sizePolygon( x )-1); y++ )
            {
            _faces.push_back( objectFace( _points[arr.getPolygonVertexPoint( x, 0 )],
                                          _points[arr.getPolygonVertexPoint( x, y )],
                                          _points[arr.getPolygonVertexPoint( x, y+1 )]) );
            }
        }

    // mesh should now be valid!
    ASSERT_MESH_IS_VALID( this );
    }

void object::setupOctree()
    {
    // build an octree from out faces
    _octree.contents().setup( _faces );
    }

void object::refreshOctree()
    {
    // refresh octree bounding boxes
    if( _octreeNeedsRefresh )
        {
        FOREACH( _faces, f )
            {
            // refresh bounding cube first
            f->refreshBoundingCube();
            }
        // well do this when required to collide ( object bounding box test confirmed )
        _octreeNeedsRefresh = FALSE;
        }
    }

void object::refreshBoundingCube()
    {
    // empty our bounds
    _bounds.clear();
    // re suggest all our points
    FOREACH( _points, p )
        {
        _bounds.suggest( p->position() );
        }
    // now our octree needs a refresh
    _octreeNeedsRefresh = TRUE;
    }

void object::integrate( pReal t )
    {
    // calculate all spring forces first
    for( int x=0; x<(int)_springs.size(); x++ )
        {
        if( _springs[x].update( t ) )
            {
            // tear if the spring has ripped
            tear( _springs[x] );
            }
        }
    // integrate all the points
    for( int x=0; x<(int)_points.size(); x++ )
        {
        _points[x].calculate( t );
        }
    ASSERT_MESH_IS_VALID( this );
    }

void object::collisions( object *in, collisionArray &arr )
    {
    // make both octree and bounding boxes fresh
    refreshOctree();
    in->refreshOctree();
    // if we're within bounds
    if( in->_bounds.test( this->_bounds ) && in->_faces.size() )
        {
        // foreach face
        for( int x=0; x<(int)_faces.size(); x++ )
            {
            // for each of the other faces
            for( int y=0; y<(int)in->_faces.size(); y++ )
                {
                // if they arent the same face ( object self-collisions )
                if( x != y )
                    {
                    // test for nice collisions ( slow and nasty )
                    plane disp;

                    if( _faces[x].collision( in->_faces[y], disp ) )
                        {
                        // append to list if we fidn one
                        arr.add( _faces[x], in->_faces[y], material().restitution() * in->material().restitution(), disp );
                        }
                    }
                }
            }
        }
    }

object *object::ground( const springMaterial &mat, pReal x, pReal y )
    {
    // create a ground object, basically make a polymesh which is a ground quad at the requested size...
    x /= 2;
    y /= 2;
    jLib::graphics::primitives::polygonMesh cloud( new jLib::graphics::primitives::triangleArray( 0 ) );
    cloud.resizePolygonArray( 1 );
    cloud.resizePolygon( 0, 4 );

    cloud.textures.add( 0.0 );
    cloud.normals.add( 0.0 );
    cloud.colours.add( 0.0 );

    cloud.points.add( position3( -x, 0, -y ) );
    cloud.points.add( position3( -x, 0, y ) );
    cloud.points.add( position3( x, 0, y ) );
    cloud.points.add( position3( x, 0, -y ) );

    cloud.setPolygonVertexPoint( 0, 0, 0 );
    cloud.setPolygonVertexPoint( 0, 1, 1 );
    cloud.setPolygonVertexPoint( 0, 2, 2 );
    cloud.setPolygonVertexPoint( 0, 3, 3 );

    cloud.setPolygonVertexTexture( 0, 0, 0 );
    cloud.setPolygonVertexTexture( 0, 1, 0 );
    cloud.setPolygonVertexTexture( 0, 2, 0 );
    cloud.setPolygonVertexTexture( 0, 3, 0 );

    cloud.setPolygonVertexColour( 0, 0, 0 );
    cloud.setPolygonVertexColour( 0, 1, 0 );
    cloud.setPolygonVertexColour( 0, 2, 0 );
    cloud.setPolygonVertexColour( 0, 3, 0 );

    cloud.setPolygonVertexNormal( 0, 0, 0 );
    cloud.setPolygonVertexNormal( 0, 1, 0 );
    cloud.setPolygonVertexNormal( 0, 2, 0 );
    cloud.setPolygonVertexNormal( 0, 3, 0 );
    cloud.rebuild();

    // then build and return it with the requested material
    return new object( mat, cloud, 0.0, 0.0 );
    }

spring *object::findSpring( particle &inA, particle &inB )
    {
    // search for the spring connnecting these two particles
    FOREACH( _springs, p )
        {
        // if they are them ( in either order ), return them!
        if( ( &(p->primary()) == &inA && &(p->secondary()) == &inB ) ||
            ( &(p->primary()) == &inB && &(p->secondary()) == &inA ) )
            {
            return &(*p);
            }
        }
    return 0;
    }

void object::facesFromSpring( spring &spr, objectFace *&fA, objectFace *&fB )
    {
    // extract the faces connected to this spring ( if they both arent non-null, mesh is broked )
    fA = fB = 0;
    bool second = FALSE;

    // for each face
    FOREACH( _faces, f )
        {
        // if any of the faces points are the springs primary
        if( f->particleA() == &(spr.primary()) ||
                f->particleB() == &(spr.primary()) ||
                f->particleC() == &(spr.primary()) )
            {
            // and the another point is the springs secondary
            if( f->particleA() == &(spr.secondary()) ||
                    f->particleB() == &(spr.secondary()) ||
                    f->particleC() == &(spr.secondary()) )
                {
                //find if we already found a face, if se return, otherwise keep going!
                if( second )
                    {
                    fB = &(*f);
                    return;
                    }
                else
                    {
                    second = TRUE;
                    fA = &(*f);
                    }
                }
            } // or secondary...
        else if( f->particleA() == &(spr.secondary()) ||
                f->particleB() == &(spr.secondary()) ||
                f->particleC() == &(spr.secondary()) )
            {
            // and primary
            if( f->particleA() == &(spr.primary()) ||
                    f->particleB() == &(spr.primary()) ||
                    f->particleC() == &(spr.primary()) )
                {
                //find if we already found a face, if se return, otherwise keep going!
                if( second )
                    {
                    fB = &(*f);
                    return;
                    }
                else
                    {
                    second = TRUE;
                    fA = &(*f);
                    }
                }
            }
        }
    // reaching here means we didnt find two faces... bad.
    jAssert( 0 );
    }

objectFace *object::otherFaceFromSpring( spring &spr, objectFace *other )
    {
    // find the other connected face from a spring and a face
    objectFace *a, *b;
    facesFromSpring( spr, a, b );
    if( a == other )
        {
        return b;
        }
    return a;
    }

void object::intersectPlane( objectFace *face, const plane &pla, springCollision &aOut, springCollision &bOut )
    {
    // find the springs for this face
    spring *a, *b, *c;
    findSprings( face, a, b, c );
    // create lines along these springs
    line lA( a->primary().position(), ( a->secondary().position() - a->primary().position() ) );
    line lB( b->primary().position(), ( b->secondary().position() - b->primary().position() ) );
    line lC( c->primary().position(), ( c->secondary().position() - c->primary().position() ) );

    bool aValid = FALSE;
    // if A Collides
    pReal iA( pla.collision( lA ) );
    if( iA >= 0.0 && iA <= 1.0 )
        {
        aValid = TRUE;
        }
    bool bValid = FALSE;
    // if B Collides
    pReal iB( pla.collision( lB ) );
    if( iB >= 0.0 && iB <= 1.0 )
        {
        bValid = TRUE;
        }

    // return if they both hit!
    if( aValid && bValid )
        {
        aOut.intersectedSpring = a;
        aOut.t = iA;
        bOut.intersectedSpring = b;
        bOut.t = iB;
        return;
        }

    // else test the last, and return the two which did hit.
    bool cValid = FALSE;
    pReal iC( pla.collision( lC ) );
    if( iC >= 0.0 && iC <= 1.0 )
        {
        cValid = TRUE;
        }

    if( cValid && bValid )
        {
        aOut.intersectedSpring = c;
        aOut.t = iC;
        bOut.intersectedSpring = b;
        bOut.t = iB;
        return;
        }

    if( aValid && cValid )
        {
        aOut.intersectedSpring = a;
        aOut.t = iA;
        bOut.intersectedSpring = c;
        bOut.t = iC;
        return;
        }
    }

object::springCollision object::intersectFace( objectFace *face, plane pla, spring *notSpr )
    {
    springCollision ret;
    // find the faces springs
    spring *a, *b, *c;
    findSprings( face, a, b, c );
    // find the two we are interested in
    if( a == notSpr )
        {
        a = c;
        }
    if( b == notSpr )
        {
        b = c;
        }
    c = 0;

    // create lines from them
    line lA( a->primary().position(), ( a->secondary().position() - a->primary().position() ) );
    line lB( b->primary().position(), ( b->secondary().position() - b->primary().position() ) );

    // if A intersects pla
    pReal iA( pla.collision( lA ) );
    if( iA >= 0.0 && iA <= 1.0 )
        {
        // return the collision
        ret.intersectedSpring = a;
        ret.t = iA;
        return ret;
        }

    // or if b did.
    pReal iB( pla.collision( lB ) );
    if( iB >= 0.0 && iB <= 1.0 )
        {
        // return the collision
        ret.intersectedSpring = b;
        ret.t = iB;
        return ret;
        }

    // this is another bad mesh indicator.
    frameLog( "intersectFace error");
    ret.intersectedSpring = 0;
    ret.t = 0;
    return ret;
    }

void object::findSprings( objectFace *face, spring *&a, spring *&b, spring *&c )
    {
    // find the springs from a face
    a = findSpring( *(face->particleA()), *(face->particleB()) );
    b = findSpring( *(face->particleB()), *(face->particleC()) );
    c = findSpring( *(face->particleC()), *(face->particleA()) );

    // if any of them are invalid, make an attempt to fix it and print debug messages
    if( a == 0 )
        {
        _springs << spring( *face->particleA(), *face->particleB( ), material() );
        a = &(_springs.back());
        qDebug() << "Error, non triangle found. attempting to fix.";
        }
    if( b == 0 )
        {
        _springs << spring( *face->particleB(), *face->particleB( ), material() );
        b = &(_springs.back());
        qDebug() << "Error, non triangle found. attempting to fix.";
        }
    if( c == 0 )
        {
        _springs << spring( *face->particleC(), *face->particleA( ), material() );
        c = &(_springs.back());
        qDebug() << "Error, non triangle found. attempting to fix.";
        }
    }

inline position3 springPosition( spring *spr, pReal t )
    {
    // interpolate along a spring
    return spr->primary().position() + t * ( spr->secondary().position() - spr->primary().position() );
    }

inline vector3 springVelocity( spring *spr, pReal t )
    {
    // interpolate between two spring velocities
    return spr->primary().velocity() + t * ( spr->secondary().velocity() - spr->primary().velocity() );
    }

void object::tear( spring &in )
    {
    // create a normal plane to the spring
    vector3 tearNormal( ( in.secondary().position() - in.primary().position() ).normalise() );

    // tear along it with some sensible parameters ( with more time this could be Gui controlled and coudl forward rips to already ripped areas etc. )
    applyTear( in, 0.5, tearNormal, 0.20 );
    }

inline bool conforms( pVector a, pVector b )
    {
    // test two vectore are in the same hemisphere
    return a.dot( b ) >= 0.0;
    }

void object::capInsertion( plane tearPlane, particle *other, particle *top, particle *bottom, particle *cutTop, particle *cutPoint, particle *cutBottom, pVector conform )
    {
    // cap a point in a tear when the rip enters the surface, leaving a point in the middle of a triangle
    bool enter = conforms( objectFace( *other, *bottom, *cutPoint ).normal(), conform );
    // if we are orientated top heavily
    if( tearPlane.isInFront( other->position() ) )
        {
        // the rip direction is entering or exiting
        if( enter )
            {
            addFace( other, bottom, cutPoint, material() );
            addFace( cutPoint, bottom, cutTop, material() );
            addFace( cutPoint, cutBottom, top, material() );
            addFace( cutPoint, top, other, material() );
            }
        else
            {
            addFace( other, cutPoint, bottom, material() );
            addFace( cutPoint, cutTop, bottom, material() );
            addFace( cutPoint, top, cutBottom, material() );
            addFace( cutPoint, other, top, material() );
            }
        }
    else // else oriented bottom heavily
        {
        // the rip direction is entering or exiting
        if( enter )
            {
            addFace( cutBottom, top, cutPoint, material() );
            addFace( other, cutPoint, top, material() );
            addFace( other, bottom, cutPoint, material() );
            addFace( bottom, cutTop, cutPoint, material() );
            }
        else
            {
            addFace( cutBottom, cutPoint, top, material() );
            addFace( other, top, cutPoint, material() );
            addFace( other, cutPoint, bottom, material() );
            addFace( bottom, cutPoint, cutTop, material() );
            }
        }
    }

void object::splitFaces( plane tearPlane, edgeList &list, QList <particle *> &pRing1, QList <particle *> &pRing2, QList <spring *> &sRing1, QList <spring *> &sRing2 )
    {
    // cant split an invalid mesh
    ASSERT_MESH_IS_VALID( this );

    // create a point list for possible flipping.
    QList <pVector> pointList;
    FOREACH( list, l )
        {
        if( l->thisSpring )
            {
            pointList << springPosition( l->thisSpring, l->t );
            }
        else
            {
            pointList << l->pos;
            }
        }
    // if it needs flipping ( has a nagative area, wrong winding )
    if( areaPolygon( pointList, tearPlane.normal ) < 0 )
        {
        // flip the list
        edgeList listTemp( list );
        list.clear();
        while( !listTemp.isEmpty() )
            {
            list << listTemp.takeLast();
            }
        }

    // for each element in the rip
    FOREACHCONST( list, l )
        {
        // if its a normal rip point
        if( l->mode == edge::Normal )
            {
            // add a point for both points in the rip
            _points << particle( 1, springPosition( l->thisSpring, l->t ), springVelocity( l->thisSpring, l->t ) );
            pRing1 << &_points.back();

            _points << particle( 1, springPosition( l->thisSpring, l->t ), springVelocity( l->thisSpring, l->t ) );
            pRing2 << &_points.back();
            }
        else
            {
            // otherwise the two rip rings are attached at this point
            // ps, we should interpolate a velocity result for this point now
            _points << particle( 1, l->pos, 0.0 );
            pRing1 << &_points.back();
            pRing2 << &_points.back();
            }
        }

    // now for each list item again
    for( int x=0; x<list.size(); x++ )
        {
        // find some valid indexes
        int pointA( x-1 ), pointB( x );
        if( pointA < 0 )
            {
            pointA = list.size() - 1;
            }
        // if the prev point in the ring had no spring ( wasnt inside the object ) and this point has a spring
        if( !list[pointA].thisSpring && list[pointB].thisSpring )
            {
            particle *other, *a, *b;

            // find the points on this face and how they should be oriented
            if( ( list[pointA].face->particleA() == &(list[pointB].thisSpring->primary()) &&
                      list[pointA].face->particleB() == &(list[pointB].thisSpring->secondary()) ) ||
                    ( list[pointA].face->particleB() == &(list[pointB].thisSpring->primary()) &&
                      list[pointA].face->particleA() == &(list[pointB].thisSpring->secondary()) ) )
                {
                other = list[pointA].face->particleC();
                a = list[pointA].face->particleA();
                b = list[pointA].face->particleB();
                }
            else if( ( list[pointA].face->particleC() == &(list[pointB].thisSpring->primary()) &&
                       list[pointA].face->particleB() == &(list[pointB].thisSpring->secondary()) ) ||
                     ( list[pointA].face->particleB() == &(list[pointB].thisSpring->primary()) &&
                       list[pointA].face->particleC() == &(list[pointB].thisSpring->secondary())) )
                {
                other = list[pointA].face->particleA();
                a = list[pointA].face->particleC();
                b = list[pointA].face->particleB();
                }
            else if( ( list[pointA].face->particleC() == &(list[pointB].thisSpring->primary()) &&
                       list[pointA].face->particleA() == &(list[pointB].thisSpring->secondary()) ) ||
                     ( list[pointA].face->particleA() == &(list[pointB].thisSpring->primary()) &&
                       list[pointA].face->particleC() == &(list[pointB].thisSpring->secondary()) ) )
                {
                other = list[pointA].face->particleB();
                a = list[pointA].face->particleA();
                b = list[pointA].face->particleC();
                }
            // flip a and b if b is in front of tearPlane
            if( tearPlane.isInFront( b->position() ) )
                {
                particle *temp( b );
                b = a;
                a = temp;
                }

            // and cap
            capInsertion( tearPlane, other, a, b, pRing1[pointB], pRing1[pointA], pRing2[pointB], list[pointA].face->normal() );
            }
        else if( list[pointB].face && list[pointA].thisSpring && !list[pointB].thisSpring )
            {
            particle *other, *a, *b;

            // find the points on this face and how they should be oriented
            if( ( list[pointB].face->particleA() == &(list[pointA].thisSpring->primary()) &&
                      list[pointB].face->particleB() == &(list[pointA].thisSpring->secondary()) ) ||
                    ( list[pointB].face->particleB() == &(list[pointA].thisSpring->primary()) &&
                      list[pointB].face->particleA() == &(list[pointA].thisSpring->secondary()) ) )
                {
                other = list[pointB].face->particleC();
                a = list[pointB].face->particleA();
                b = list[pointB].face->particleB();
                }
            else if( ( list[pointB].face->particleC() == &(list[pointA].thisSpring->primary()) &&
                       list[pointB].face->particleB() == &(list[pointA].thisSpring->secondary()) ) ||
                     ( list[pointB].face->particleB() == &(list[pointA].thisSpring->primary()) &&
                       list[pointB].face->particleC() == &(list[pointA].thisSpring->secondary())) )
                {
                other = list[pointB].face->particleA();
                a = list[pointB].face->particleC();
                b = list[pointB].face->particleB();
                }
            else if( ( list[pointB].face->particleC() == &(list[pointA].thisSpring->primary()) &&
                       list[pointB].face->particleA() == &(list[pointA].thisSpring->secondary()) ) ||
                     ( list[pointB].face->particleA() == &(list[pointA].thisSpring->primary()) &&
                       list[pointB].face->particleC() == &(list[pointA].thisSpring->secondary()) ) )
                {
                other = list[pointB].face->particleB();
                a = list[pointB].face->particleA();
                b = list[pointB].face->particleC();
                }
            // flip a and b if b is in front of tearPlane
            if( tearPlane.isInFront( b->position() ) )
                {
                particle *temp( b );
                b = a;
                a = temp;
                }

            // and cap
            capInsertion( tearPlane, other, a, b, pRing1[pointA], pRing1[pointB], pRing2[pointA], list[pointB].face->normal() );
            }
        }

    // for each element of the list
    for( int x=0; x<list.size(); x++ )
        {
        // if this element has a spring ie is on the original surface of the mesh
        if( list[x].thisSpring )
            {
            particle *old;
            // find the point in front of the tear plane ( should line to pRing1 )
            if( tearPlane.isInFront( list[x].thisSpring->primary().position() ) )
                {
                old = &(list[x].thisSpring->primary());
                list[x].thisSpring->primary( *pRing1[x] );
                }
            else
                {
                old = &(list[x].thisSpring->secondary());
                list[x].thisSpring->secondary( *pRing1[x] );
                }
            // add a space spring and attach to the other old particle and pRing2
            _springs << spring( *old, *pRing2[x], material() );
            _springs.back().internal( FALSE );
            // divide any plastic deformation the system has
            list[x].thisSpring->divideDeformationsAndReset( _springs.back() );
            }
        }

    bool flip = FALSE;
// this code is meant to combat flipped rips, but it isnt correct yet.
#if 0
    for( int x=0; x<list.size(); x++ )
        {
        int pointA( x-1 ), pointB( x ), pointZ( x-2 );
        if( pointA < 0 )
            {
            pointA = list.size() - 1;
            }
        if( pointZ < 0 )
            {
            pointZ = list.size() - 2;
            }
        if( list[pointA].thisSpring && list[pointB].thisSpring && !conforms( list[x].face->normal(), ( pRing2[pointB]->position() - list[pointB].face->pointA() ).cross( pRing2[pointA]->position() - list[x].face->pointA() ) ) )
            {
            flip = TRUE;
            }
        }
#endif

    for( int x=0; x<list.size(); x++ )
        {
        int pointA( x-1 ), pointB( x ), pointZ( x-2 );
        // if we are correcting a flipped rip
        if( flip )
            {
            // flip
            pointA = x;
            pointB = x + 1;
            }
        //get some valid indexes
        if( pointA < 0 )
            {
            pointA = list.size() - 1;
            }
        if( pointB >= list.size() )
            {
            pointB -= list.size();
            }
        if( pointZ < 0 )
            {
            pointZ = list.size() - 2;
            }

        // if this point and the previous are surface faces
        if( list[pointA].thisSpring && list[pointB].thisSpring )
            {
            // rearrange the old face into a new valid face, and create a quad the needs filling
            // this unfortunately vaires depending on the faces orientation
            particle *newFaces[4];
            if( tearPlane.isInFront( list[x].face->pointA() ) &&
                !tearPlane.isInFront( list[x].face->pointB() ) &&
                !tearPlane.isInFront( list[x].face->pointC() ) )
                {
                newFaces[1] = list[x].face->particleC();
                newFaces[0] = list[x].face->particleB();
                newFaces[2] = pRing1[pointA];
                newFaces[3] = pRing1[pointB];
                list[x].face->particleB( pRing2[pointA] );
                list[x].face->particleC( pRing2[pointB] );
                }
            else if( tearPlane.isInFront( list[x].face->pointC() ) &&
                !tearPlane.isInFront( list[x].face->pointB() ) &&
                !tearPlane.isInFront( list[x].face->pointA() ) )
                {
                newFaces[1] = list[x].face->particleB();
                newFaces[0] = list[x].face->particleA();
                newFaces[2] = pRing1[pointA];
                newFaces[3] = pRing1[pointB];
                list[x].face->particleA( pRing2[pointA] );
                list[x].face->particleB( pRing2[pointB] );
                }
            else if( tearPlane.isInFront( list[x].face->pointB() ) &&
                !tearPlane.isInFront( list[x].face->pointA() ) &&
                !tearPlane.isInFront( list[x].face->pointC() ) )
                {
                newFaces[1] = list[x].face->particleC();
                newFaces[0] = list[x].face->particleA();
                newFaces[3] = pRing1[pointA];
                newFaces[2] = pRing1[pointB];
                list[x].face->particleC( pRing2[pointA] );
                list[x].face->particleA( pRing2[pointB] );
                }
            else if( tearPlane.isInFront( list[x].face->pointA() ) &&
                tearPlane.isInFront( list[x].face->pointB() ) &&
                !tearPlane.isInFront( list[x].face->pointC() ) )
                {
                newFaces[0] = list[x].face->particleA();
                newFaces[1] = list[x].face->particleB();
                newFaces[2] = pRing2[pointA];
                newFaces[3] = pRing2[pointB];
                list[x].face->particleA( pRing1[pointA] );
                list[x].face->particleB( pRing1[pointB] );
                }
            else if( tearPlane.isInFront( list[x].face->pointC() ) &&
                tearPlane.isInFront( list[x].face->pointB() ) &&
                !tearPlane.isInFront( list[x].face->pointA() ) )
                {
                newFaces[0] = list[x].face->particleB();
                newFaces[1] = list[x].face->particleC();
                newFaces[3] = pRing2[pointA];
                newFaces[2] = pRing2[pointB];
                list[x].face->particleC( pRing1[pointA] );
                list[x].face->particleB( pRing1[pointB] );
                }
            else if( tearPlane.isInFront( list[x].face->pointC() ) &&
                tearPlane.isInFront( list[x].face->pointA() ) &&
                !tearPlane.isInFront( list[x].face->pointB() ) )
                {
                newFaces[0] = list[x].face->particleC();
                newFaces[1] = list[x].face->particleA();
                newFaces[2] = pRing2[pointA];
                newFaces[3] = pRing2[pointB];
                list[x].face->particleC( pRing1[pointA] );
                list[x].face->particleA( pRing1[pointB] );
                }
            // finally add the faces requested
            addFace( newFaces[0], newFaces[1], newFaces[2], material() );
            addFace( newFaces[1], newFaces[3], newFaces[2], material() );
            }
        else if( list[pointA].face && list[pointB].thisSpring ) // else if the previous face holds a face and no spring ( came from inside the mesh )
            {
            FOREACH( _faces, f )
                {
                if( &(*f) == list[pointA].face )
                    {
                    // we erase this face, as it will has already been filled nicely above
                    _faces.erase( f );
                    break;
                    }
                }
            }
        else if( list[pointB].face && list[pointA].thisSpring ) // or if the next edge is diving into the mesh
            {
            FOREACH( _faces, f )
                {
                if( &(*f) == list[pointB].face )
                    {
                    // remove this face as it was already filled above
                    _faces.erase( f );
                    break;
                    }
                };
            }
        }

    // finally for each point in the ring
    for( int x=1; x<=pRing1.size(); x++ )
        {
        // valid indexes
        int pointA( x - 1 ), pointB( x % pRing1.size() );

        // add springs per edge
        _springs << spring( *pRing1[pointA], *pRing1[pointB], material() );
        _springs.back().internal( FALSE );
        sRing1 << &_springs.back();
        // only one spring if the rings are joined at this point
        if( pRing1[pointB] != pRing2[pointB] || pRing1[pointA] != pRing2[pointA] )
            {
            _springs << spring( *pRing2[pointA], *pRing2[pointB], material() );
            _springs.back().internal( FALSE );
            }
        sRing2 << &_springs.back();
        }

    // ensure the mass is evenly spread
    redistributeMass( );

    // mesh should still be valid
    ASSERT_MESH_IS_VALID( this );
    }

void object::redistributeMass( )
    {
    // would be nice if this calculated volume and distributed nicely
    pReal mPP;
    // dont allow NaN mass...
    if( material().density() == jLib::floatLimits::infinity() )
        {
        mPP = jLib::floatLimits::infinity();
        }
    else
        {
        mPP = ( material().density() * _bounds.volume() ) / _points.size();
        }

    // Set point masses
        {
        FOREACH( _points, p )
            {
            if( p->mass() != jLib::floatLimits::infinity() )
            p->mass( mPP );
            }
        }
    }

QList <objectFace *> object::capHole( const QList <particle *> &listRef, vector3 planeNormal )
    {
    QList <particle *> list( listRef );
    QList <objectFace *> ret;

    int point( 0 );
    // cyclic filling, only filling triangles that face the right way, this algorithm doesnt create bad polygons, but isnt the best capping algorithm.
    while( list.size() > 3 )
        {
        // nice indexes
        int plusOne( ( point + 1 ) % list.size() ), plusTwo( ( point + 2 ) % list.size() );

        // if the triangle faces the right way
        if( doubleTriangleArea( list[point], list[plusOne], list[plusTwo] ) > 0 )
            {
            // insert a face
            addFace( list[point], list[plusOne], list[plusTwo], material() );
            // add the created face to the return list
            ret << &(_faces.back());
            // weve accounted for this point, remove it now
            list.removeAt( plusOne );
            }
        // increment list
        point = (point+1) % list.size();
        }

    // cap the final face
    addFace( list[0], list[1], list[2], material() );
    // add final face to the list
    ret << &(_faces.back());
    return ret;
    }

QList <spring *> object::intersectWithSprings( const QList <objectFace *> &faces )
    {
    QList <spring *> ret;
    // for each face
    FOREACHCONST( faces, f )
        {
        // find a triangle from this face
        triangle tri( (*f)->pointA(), (*f)->pointB(), (*f)->pointC() );
        // and a plane
        plane pla( (*f)->pointA(), pVector( (*f)->pointB() - (*f)->pointA() ).cross( (*f)->pointC() - (*f)->pointA() ).normalise() );
        // for each spring
        for( int x=0; x<_springs.size(); x++ )
            {
            // only if its internal
            if( _springs[x].internal() )
                {
                // find the line of the spring
                line li( _springs[x].primary().position(), _springs[x].secondary().position() - _springs[x].primary().position() );
                pReal t( pla.collision( li ) );
                // test for a real collision
                if( t >= 0.0 && t <= 1.0 && tri.liesOn( li.sample( t ) ) )
                    {
                    // add the the return list
                    ret << &(_springs[x]);
                    }
                }
            }
        }
    return ret;
    }

void object::splitSpringsAndSnapToParticles( pReal t, const QList <spring *> &intersectSprings, const QList <particle *> &pRing1, const QList <particle *> &pRing2, plane tearPlane )
    {
    // for each spring to intersect
    FOREACHCONST( intersectSprings, s )
        {
        // find a map of distances from the point t on the spring to each particle
        QMap <pReal, particle *> distanceMapA, distanceMapB;
        position3 pos( springPosition( *s, t ) );
        FOREACHCONST( pRing1, p )
            {
            distanceMapA.insert( ( (*p)->position() - pos ).length(), *p );
            }
        // for both rings
        FOREACHCONST( pRing2, p )
            {
            distanceMapB.insert( ( (*p)->position() - pos ).length(), *p );
            }

        // find the furthest particle
        particle *frontA( *(--distanceMapA.end() ) );
        particle *old;
        // attach the spring-particle in front of the plane the pRing1, and the other to pRing2
        if( tearPlane.isInFront( (*s)->primary().position() ) )
            {
            // store other value, and rearrange old spring
            old = &((*s)->primary());
            (*s)->primary( *frontA );
            }
        else
            {
            // store other value, and rearrange old spring
            old = &((*s)->secondary());
            (*s)->secondary( *frontA );
            }
        // add the other spring to the array
        _springs << spring( *old, **(--distanceMapB.end()), material() );
        }
    }

void object::tearWedge( springCollision collision, objectFace *startFace, plane tearPlane, pReal radius, edgeList &list )
    {
    // this could be controllable, for now 3 is a good value
    unsigned int sections( 3 );
    pVector startPos( springPosition( collision.intersectedSpring, collision.t ) );

    // find a line for the initial internal rip, if we have no actual normal rips, then find one manually
    line li;
    if( !list.isEmpty() && list.back().thisSpring )
        {
        li = line( startPos, springPosition( list.back().thisSpring, list.back().t ) - startPos );
        }
    else if( !list.isEmpty() )
        {
        li = line( startPos, list.back().pos - startPos );
        }
    else
        {
        li = line( startPos, tearPlane.position - startPos );
        }

    // create a sphere for collision
    sphere sph( tearPlane.position, radius );
    pReal a, b;
    sph.collision( li, a, b );
    pReal intersection = a;
    if( b <= 1.0 && b >= 0.0 )
        {
        intersection = b;
        }
    // set up some initial ripping things
    jLib::math::triple insertVector( li.sample( intersection ) - tearPlane.position );
    jLib::math::quaternion quat( jLib::math::quaternion::fromAxisAngle( tearPlane.normal, JM2xPI / sections ) );
    jLib::math::matrix4x4 rotator;
    rotator.setOrientation( quat );

    QList <pVector> intersectionList;
    // for each requried rip section
    for( unsigned int x=0; x<=sections; x++ )
        {
        // append the original position plus the rip vector
        intersectionList << tearPlane.position + insertVector;
        // rotate the vector by the correct amount
        insertVector = rotator.vectorMultiply( insertVector );
        }

    //if the rip lists' winding is incorrect
    if( startFace->normal().dot( intersectionList[1] - intersectionList[0] ) > 0 )
        {
        // flip it.
        QList <pVector> temp( intersectionList );
        intersectionList.clear();
        while( temp.size() )
            {
            intersectionList << temp.takeLast();
            }
        }

    pVector endPosition;
    objectFace *endFace = 0;
    unsigned int endPoint = 0;
    // now for each section
    for( unsigned int x=0; x<sections; x++ )
        {
        bool exit = false;
        // create a line from this rip point to the next rip point
        line thisLine( intersectionList[x], intersectionList[x+1]-intersectionList[x] );
        // test for collision with all the faces in the array ( for exit of the internal rip )
        FOREACH( _faces, f )
            {
            // if its not the internal rip face ( cant exit on the entry rip point )
            if( &(*f) != startFace )
                {
                // try to collide
                triangle tri( f->pointA(), f->pointB(), f->pointC() );
                pReal collision;
                // if we collide
                if( tri.collision( thisLine, &collision ) && collision >= 0.01 && collision <= 1.05 )
                    {
                    // store the information and exit
                    endPosition = thisLine.sample( collision );
                    endPoint = x;
                    endFace = &(*f);
                    exit = true;
                    }
                }
            }
        if( exit == true )
            {
            break;
            }
        }

    // add the start point to the list
    list << edge( startFace, li.sample( intersection ) );

    // add the intermediate points
    for( unsigned int x=1; x<=endPoint; x++ )
        {
        list << edge( intersectionList[x] );
        }

    // add the end point
    list << edge( endFace, endPosition );
    }

void object::applyTear( spring &spr, pReal t, vector3 tearNormal, pReal tearRadius )
    {
    static int torn = 0; // static tear limit for debugging
    jAssert( t >= 0.0 );
    jAssert( t <= 1.0 );

    // create a tear plane
    plane pla( springPosition( &spr, t ), tearNormal );

    // find a start plane
    objectFace *start = otherFaceFromSpring( spr, 0 );
    if( start != 0 && torn < 10 ) // ensure we can rip
        {
        torn++;
        // set up some variables
        edgeList list;
        objectFace *previous( start );
        springCollision collision;
        collision.intersectedSpring = &spr;
        // mesh should be valid now
        ASSERT_MESH_IS_VALID( this );
        do
            {
            if( !previous ) // previous shoudl always be true
                {
                qDebug() << "Tear face error, bailing";
                return;
                }
            // find the next collision
            collision = intersectFace( previous, pla, collision.intersectedSpring );

            // it should be a valid collision
            if( !collision.intersectedSpring )
                {
                ASSERT_MESH_IS_VALID( this );
                }

            // find the position of the current rip
            pVector nowPos( springPosition( collision.intersectedSpring, collision.t ) );
            // if it goes out of the rip radius
            if( ( pla.position - nowPos ).length() > tearRadius )
                {
                // tear a wedge
                tearWedge( collision, previous, pla, tearRadius, list );

                // and fix the list so we can continue as normal with rip
                springCollision a, b;
                intersectPlane( list.back().face, pla, a, b );
                if( ( springPosition( a.intersectedSpring, a.t ) - pla.position ).length() >
                        ( springPosition( b.intersectedSpring, b.t ) - pla.position ).length() )
                    {
                    // add a valid previous entry with a spring to the list
                    list << edge( list.back().face, b.intersectedSpring, b.t );
                    collision = b;

                    }
                else
                    {
                    // add a valid previous entry with a spring to the list
                    list << edge( list.back().face, a.intersectedSpring, a.t );
                    collision = a;
                    }

                // set previous to a good value too
                previous = list.back().face;
                }
            else
                {
                // otherwise, normal rip occured, add the edge to the list
                list << edge( previous, collision.intersectedSpring, collision.t );
                }

            // find the next rip
            previous = otherFaceFromSpring( *collision.intersectedSpring, previous );

            // contine until we find our way back to start
            } while( previous != start );

        // mesh should be valid now too.
        ASSERT_MESH_IS_VALID( this );

        QList <particle *> pRing1;
        QList <particle *> pRing2;
        QList <spring *> sRing1;
        QList <spring *> sRing2;

        // split the faces found by the ripping
        splitFaces( pla, list, pRing1, pRing2, sRing1, sRing2 );

        ASSERT_MESH_IS_VALID( this );

        // find a flipped ring
        QList <particle *> pRingTemp;
        FOREACH( pRing1, p )
            {
            pRingTemp.push_front( *p );
            }

        // cap the hole inverted ( for correct normals )
        capHole( pRingTemp, 0.0 );

        ASSERT_MESH_IS_VALID( this );

        // cap the olther hold in the normal direction
        QList <objectFace *> intersectFaces( capHole( pRing2, 0.0 ) );

        ASSERT_MESH_IS_VALID( this );

        // find the intersected springs
        QList <spring *> intersectSprings( intersectWithSprings( intersectFaces ) );

        // split the springs and attach them to sensible particles.
        splitSpringsAndSnapToParticles( t, intersectSprings, pRing1, pRing2, pla );

        ASSERT_MESH_IS_VALID( this );
        }
    }

void object::testFaceSpringLinks()
    {
    // for each face
    FOREACH( _faces, f )
        {
        // try to find 3 associated springs
        spring *a, *b, *c;
        findSprings( &(*f), a, b, c );
        // otherwise somethign is wrong
        if( !a || !b || !c )
            {
            qDebug() << "Missing Spring";
            }
        }
    }


void object::addFace( particle *a, particle *b, particle *c, const springMaterial &mat )
    {
    // add the face
    _faces << objectFace( *a, *b, *c );

    // ensure it is added with valid faces
    if( !findSpring( *a, *b ) )
        {
        _springs << spring( *a, *b, mat );
        }
    if( !findSpring( *a, *c ) )
        {
        _springs << spring( *a, *c, mat );
        }
    if( !findSpring( *b, *c ) )
        {
        _springs << spring( *b, *c, mat );
        }
    }

void object::addSpring( particle *a, particle *b, const springMaterial &mat, bool inter )
    {
    // test this spring is unique
    if( !findSpring( *a, *b ) )
        {
        // add it
        _springs << spring( *a, *b, mat );
        _springs.back().internal( inter );
        }
    }

END_PHYSICAL_NAMESPACE
