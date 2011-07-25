#ifndef MATERIALENTITY_H
#define MATERIALENTITY_H

#include "AEntity"

// an object material
class materialEntity : public AEntity
    {
    A_ENTITY( materialEntity, "physical.placid-acid.com", "PhysiCal/Material" )

public:
    // create a material
    materialEntity();
    };

#endif // MATERIALENTITY_H
