#include "materialFront.h"
#include "../SDK/appLog.h"
#include "physiCal.h"

materialFront::materialFront( ) : propertyEntity( )
    {
    addProperty( "material/density", 1.0f, "Density" );
    addProperty( "material/elasticLimit", jLib::floatLimits::infinity(), "Elastic Limit" );
    addProperty( "material/ultimateTensileStress", jLib::floatLimits::infinity(), "Ultimate Tensile Stress" );
    addProperty( "material/edgeSpringConstant", 1.0f, "Edge Spring Constant" );
    addProperty( "material/diagonalSpringConstant", 1.0f, "Diagonal Spring Constant" );
    addProperty( "material/volumeSpringConstant", 1.0f, "Volume Spring Constant" );
    }

materialFront::~materialFront( )
    {
    }

propertyEntity *materialFront::create( )
    {
    return new materialFront( );
    }

void materialFront::propertyChanged( QString name )
    {
    if( name == "material/density" ||
        name == "material/elasticLimit" ||
        name == "material/ultimateTensileStress" ||
        name == "material/edgeSpringConstant" ||
        name == "material/diagonalSpringConstant" ||
        name == "material/volumeSpringConstant" )
        {
        property( name ) = property( name )->toDouble();
        }
    else
        {
        propertyEntity::propertyChanged( name );
        }
    }
