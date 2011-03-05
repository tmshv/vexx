#ifndef LATTICEFRONT_H
#define LATTICEFRONT_H

#include "../SDK/appLog.h"
#include "../SDK/viewportEntity.h"

class latticeFront : public viewportEntity
    {
    DEFINE_CLASS_TYPE( "physical.placid-acid.com", "Physics/Lattice" )
public:
    static propertyEntity *create( );

    virtual void propertyChanged( QString );

private:
    latticeFront( );
    jLib::graphics::primitives::polygonMesh _mesh;
    };

#endif // LATTICEFRONT_H
