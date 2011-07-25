#ifndef LATTICETREELEAF_H
#define LATTICETREELEAF_H

#include "physiCalGlobal.h"
#include "octree.h"
#include "particle.h"
#include "boundingCube.h"

BEGIN_PHYSICAL_NAMESPACE

class PHYSICAL_EXPORT latticeTreeLeaf
    {
    jROProperty( octree <latticeTreeLeaf> *, parent )
public:
    latticeTreeLeaf( octree <latticeTreeLeaf> *p );
    void distribute( QList <particle> & );
    void calculateLeafBoundsFromParent( );


private:
    jLib::vector <particle *> _particles;
    boundingCube _bounds;
    };

END_PHYSICAL_NAMESPACE

#endif
