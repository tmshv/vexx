#ifndef JLIB_GRAPHICS_PRIMITIVES_POINTCLOUD_H
#define JLIB_GRAPHICS_PRIMITIVES_POINTCLOUD_H

#include "graphics/primitives/graphics.primitives.types.h"
#include "graphics/primitives/graphics.primitives.pointArray.h"
#include "graphics/entity/graphics.entity.entityG.h"

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

class JLIB_EXPORT pointCloud : public pointArray
    {
public:
    pointCloud( entity::entityG *, float s=1 );

    virtual void removePoint( unsigned int );
    unsigned int addPoint( math::triple, math::colour4=math::colour4() );
    virtual void clear( );
    virtual void clearPoints( );

protected:
    unsigned int _usedPoints;
    };

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

#endif
