#ifndef GROUNDOBJECT_H
#define GROUNDOBJECT_H

#include "viewportEntity.h"

//! an object that represents the ground
class groundObject : public viewportEntity
    {
    A_ENTITY( groundObject, "physical.placid-acid.com", "PhysiCal/Ground Object" )

public:
    // create a basic ground model
    groundObject();
    // update the ground draw mesh
    void update();

private:
    jLib::graphics::primitives::triangleCloud _cloud;
    };

#endif // GROUNDOBJECT_H
