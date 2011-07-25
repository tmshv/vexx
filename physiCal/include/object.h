#ifndef OBJECT_H
#define OBJECT_H

#include "physiCalGlobal.h"
#include "particle.h"
#include "spring.h"
#include "objectFace.h"
#include "collisionArray.h"
#include "boundingCube.h"
#include "octree.h"
#include "objectLeaf.h"

BEGIN_PHYSICAL_NAMESPACE

struct edge;

typedef QList <edge> edgeList;

//! Represents an physical object in the world
class PHYSICAL_EXPORT object
    {
public:
    //! A reference to the material for this object
    jPtrProperty( const springMaterial, material );
    //! The springs in this object
    jRefProperty( QList <spring>, springs );
    //! The points in this mesh
    jRefProperty( QList <particle>, points );
    //! The faces in this mesh
    jRefProperty( QList <objectFace>, faces );

public:
    //! Create an object from a polymesh and material
    object( const springMaterial &material,
            const jLib::graphics::primitives::polygonMesh &mesh,
            position3 position=0.0,
            vector3 velocity=0.0,
            unsigned int extraParticlesPerUnitVolume=0,
            unsigned int extraSpringsPerPoint=0,
            float maxInternalSpringLength=1.0 );

    //! Refresh this objects boundingCube
    void refreshBoundingCube();
    //! find collisions between this object and the passed object, put them in the array passed
    void collisions( object *obj, collisionArray &array );
    //! Step forward by the given time
    void integrate( pReal time );

    //! Create a ground object from the material given, of size x by y
    static object *ground( const springMaterial &mat, pReal x, pReal y );

private:
    //! Add face linked by the given particles and material
    void addFace( particle *, particle *, particle *, const springMaterial & );
    //! Add the spring from particle a to b
    void addSpring( particle *a, particle *b, const springMaterial &mat, bool internal );

    struct springCollision { spring *intersectedSpring; pReal t; };
    void tear( spring & );
    void applyTear( spring &spr, pReal t, vector3 tearNormal, pReal tearRadius );
    void setupOctree();
    void refreshOctree();
    bool _octreeNeedsRefresh;
    octree <objectLeaf> _octree;
    boundingCube _bounds;
    object( const object & ) { }
    object &operator=( const object & ) { return *this; }

    void facesFromSpring( spring &, objectFace *&, objectFace *& );
    objectFace *otherFaceFromSpring( spring &, objectFace * );
    spring *findSpring( particle &, particle & );
    springCollision intersectFace( objectFace *, plane pla, spring * );
    void intersectPlane( objectFace *, const plane &, springCollision &, springCollision & );
    void findSprings( objectFace *, spring *&, spring *&, spring *& );
    void tearWedge( springCollision, objectFace *, plane, pReal, edgeList & );

    void capInsertion( plane, particle *, particle *, particle *, particle *, particle *, particle *, pVector );
    QList <spring *> intersectWithSprings( const QList <objectFace *> & );
    void splitFaces( plane tearPlane, edgeList &list, QList <particle *> &pRing1, QList <particle *> &pRing2, QList <spring *> &sRing1, QList <spring *> &sRing2 );
    QList <objectFace *> capHole( const QList <particle *> &list, vector3 planeNormal );
    void redistributeMass( );
    void splitSpringsAndSnapToParticles( pReal t, const QList <spring *> &intersectSprings, const QList <particle *> &pRing1, const QList <particle *> &pRing2, plane tearPlane );
    void testFaceSpringLinks();
    };

END_PHYSICAL_NAMESPACE

#endif
