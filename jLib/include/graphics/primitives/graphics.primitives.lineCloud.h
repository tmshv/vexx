#ifndef JLIB_GRAPHICS_PRIMITIVES_LINECLOUD_H
#define JLIB_GRAPHICS_PRIMITIVES_LINECLOUD_H

#include "graphics/primitives/graphics.primitives.types.h"
#include "graphics/primitives/graphics.primitives.lineArray.h"

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

class JLIB_EXPORT lineCloud : public lineArray
    {
public:
    lineCloud( entity::entityG *, float w=1 );

    virtual void removeLine( unsigned int );
    virtual void removePoint( unsigned int );
    unsigned int addLine( unsigned int, unsigned int );
    unsigned int addLine( math::triple, math::triple );
    unsigned int addLineLoop( unsigned int *, unsigned int );
    unsigned int addLineStrip( unsigned int *, unsigned int );
    unsigned int addLineLoop( math::triple *, math::colour4 *, unsigned int );
    unsigned int addLineStrip( math::triple *, math::colour4 *, unsigned int );
    virtual void clear( );
    virtual void clearLines( );

protected:
    unsigned int _usedLines;
    };

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

#endif
