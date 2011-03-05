#ifndef JLIB_GRAPHICS_PRIMITIVES_LINEARRAY_H
#define JLIB_GRAPHICS_PRIMITIVES_LINEARRAY_H

#include "graphics/primitives/graphics.primitives.types.h"
#include "graphics/primitives/graphics.primitives.pointCloud.h"

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

struct lineArrayPrim
    {
    unsigned int a;
    unsigned int b;
    };
class JLIB_EXPORT lineArray : public pointCloud
    {
public:
    lineArray( entity::entityG *p, float w=1 );
    virtual ~lineArray( );

    virtual void removeLine( unsigned int );
    void setLine( unsigned int, unsigned int, unsigned int );
    void setLineA( unsigned int, unsigned int );
    void setLineB( unsigned int, unsigned int );
    unsigned int getLineA( unsigned int );
    unsigned int getLineB( unsigned int );
    void setLineArraySize( unsigned int );
    unsigned int sizeLines( );
    virtual void clear( );
    virtual void clearLines( );
    virtual void draw();

protected:
    vector <struct lineArrayPrim> _lineData;
    };

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

#endif
