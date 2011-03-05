#ifndef MULTIBBOX_H
#define MULTIBBOX_H

#include "physiCalGlobal.h"
#include "particle.h"
#include "spring.h"
#include "springMaterial.h"
#include "face.h"
#include "collisionArray.h"
#include "boundingCube.h"
#include "latticeTreeLeaf.h"

BEGIN_PHYSICAL_NAMESPACE

class PHYSICAL_EXPORT lattice
    {
    jPtrProperty( const springMaterial, material )
public:
    lattice( const springMaterial &mat, const jLib::graphics::primitives::triangleArray *, const position3 &pos=position3( 0.0 ), unsigned int=1 );
    lattice( const lattice & );

    static lattice ground( const springMaterial &mat, pReal, pReal );

    QList <particle> &particles();
    const QList <particle> &particles() const;
    QList <face> &faces();
    const QList <face> &faces() const;
    void update( pReal );
    void refreshBoundingCube();

    const boundingCube &bounds() const;

    void collisions( lattice &, collisionArray & );
    void debugLines( jLib::graphics::primitives::lineCloud & );
    void debugPoints( jLib::graphics::primitives::pointCloud & );
    void debugTriangles( jLib::graphics::primitives::triangleCloud & );
    void debugNormals( jLib::graphics::primitives::lineCloud & );
    inline particle &particleAt( unsigned int x, unsigned int y, unsigned int z )
        {
        jAssert( (int)( ( z * ( ( _yMax + 1 ) * ( _xMax + 1 ) ) ) + ( y * ( _xMax + 1 ) ) + x ) < _particles.size() );
        return _particles[ ( z * ( ( _yMax + 1 ) * ( _xMax + 1 ) ) ) + ( y * ( _xMax + 1 ) ) + x ];
        }
private:
    void initFromBoundingCube();
    void initSpringsFromParticles();
    void linkFacesToParticles();
    octree <latticeTreeLeaf> _tree;
    QList <particle> _particles;
    QList <face> _faces;
    QList <spring> _springs;
    unsigned int _xMax;
    unsigned int _yMax;
    unsigned int _zMax;
    boundingCube _bounds;
    bool _octreeBoundingBoxesNeedRefresh;
    };

END_PHYSICAL_NAMESPACE

#endif
