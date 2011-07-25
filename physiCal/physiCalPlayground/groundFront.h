#ifndef GRID_H
#define GRID_H

#include "../SDK/viewportEntity.h"

class groundFront : public viewportEntity
    {
    DEFINE_CLASS_TYPE( "physical.placid-acid.com", "Physics/Ground" )
public:
    static propertyEntity *create( );

    virtual void propertyChanged( QString );

private:
    groundFront( );
    jLib::graphics::implicit::grid _grid;
    };

#endif
