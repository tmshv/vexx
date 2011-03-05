#ifndef PHYSICALENTITY_H
#define PHYSICALENTITY_H

#include "viewportEntity.h"

//! an object in the 3D world, created from a COLLADA Mesh
class objectEntity : public viewportEntity
    {
    Q_OBJECT
    A_ENTITY( objectEntity, "physical.placid-acid.com", "PhysiCal/Object" )
    jRORefProperty( jLib::graphics::primitives::polygonMesh, mesh )

public:
    objectEntity( );
    virtual void update();
    };

#endif // PHYSICALENTITY_H
