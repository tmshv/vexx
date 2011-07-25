#include "groundObject.h"

groundObject::groundObject() : _cloud( 0 )
    {
    // add Gui properties
    addProperty( "xRes", 10, propertyInfo::Double, "X Size" );
    addProperty( "zRes", 10, propertyInfo::Double, "Z Size" );
    // set up draw mesh
    object( &_cloud );
    }

void groundObject::update()
    {
    // refresh representative square
    _cloud.clearPoints();
    _cloud.clearTriangles();
    JFLOAT x( property( "xRes" )->toDouble() / 2.0 );
    JFLOAT z( property( "zRes" )->toDouble() / 2.0 );
    _cloud.addQuad( _cloud.addPoint( jLib::math::triple( -x, 0, -z ), 0, 0, 1 ),
                    _cloud.addPoint( jLib::math::triple( x, 0, -z ), 0, 0, 1 ),
                    _cloud.addPoint( jLib::math::triple( x, 0, z ), 0, 0, 1 ),
                    _cloud.addPoint( jLib::math::triple( -x, 0, z ), 0, 0, 1 ) );
    }
