#ifndef FREEZEBOX_H
#define FREEZEBOX_H

#include "viewportEntity.h"
#include "boundingBox.h"

//! A box in the 3D world for pinning verticies of a model
class freezeBox : public viewportEntity
    {
    A_ENTITY( freezeBox, "physical.placid-acid.com", "PhysiCal/Freeze Box" )

public:
    // construct an default box
    freezeBox();
    // called when the properties are changed to update the drawing mesh
    void update();

private:
    boundingBox _box;
    };

#endif // FREEZEBOX_H
