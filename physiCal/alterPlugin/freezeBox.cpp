#include "freezeBox.h"

freezeBox::freezeBox() : _box( 0, 0, 1 )
    {
    // set up draw mesh
    object( &_box );
    // add Gui properties
    addProperty( "minimum", XVector3D( 0.0, 0.0, 0.0 ), propertyInfo::Position );
    addProperty( "maximum", XVector3D( 1.0, 1.0, 1.0 ), propertyInfo::Position );
    }

void freezeBox::update()
    {
    // set up the box from properties
    _box.rebuild( property( "minimum" )->toVector3D(), property( "maximum" )->toVector3D() );
    }
