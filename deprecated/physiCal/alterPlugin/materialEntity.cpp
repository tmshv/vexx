#include "materialEntity.h"

materialEntity::materialEntity()
    {
    // add Gui properties
    addProperty( "density", 1, propertyInfo::Double, "Density" );
    addProperty( "restitution", 0.1, propertyInfo::Double, "Restitution" );
    addProperty( "springConstant", 1, propertyInfo::Double, "Spring Constant" );
    addProperty( "springDamping", 0, propertyInfo::Double, "Spring Damping" );
    addProperty( "elastic", 999, propertyInfo::Double, "Elastic Limit" );
    addProperty( "plastic", 999, propertyInfo::Double, "Plastic Limit" );
    }
