#ifndef JLIB_GRAPHICS_PRIMITIVES_TRIANGLECLOUD_H
#define JLIB_GRAPHICS_PRIMITIVES_TRIANGLECLOUD_H

#include "graphics/primitives/graphics.primitives.types.h"
#include "graphics/primitives/graphics.primitives.triangleArray.h"

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

class JLIB_EXPORT triangleCloud : public triangleArray
    {
public:
    triangleCloud( entity::entityG * );
    triangleCloud( entity::entityG *, file::geometryFileAbstract );

    unsigned int addQuad( unsigned int, unsigned int, unsigned int, unsigned int );
    unsigned int addPolygon( unsigned int *, unsigned int );
    unsigned int addTriangleFan( unsigned int *, unsigned int );
    unsigned int addTriangle( unsigned int, unsigned int, unsigned int );
    unsigned int addPoint( math::triple, math::triple=math::triple( 0.0 ), math::vector2=math::vector2( 0.0 ), math::colour4=math::colour4::WHITE );
    virtual void clearTriangles( );
    virtual void removeTriangle( unsigned int );
    virtual void removePoint( unsigned int );
    virtual void clearPoints( );

protected:
    unsigned int _usedPoints;
    unsigned int _usedTriangles;
    };

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

#endif
