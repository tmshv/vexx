#ifndef JLIB_GRAPHICS_COLLADA_BASE_H
#define JLIB_GRAPHICS_COLLADA_BASE_H

#include "graphics/collada/graphics.collada.types.h"

BEGIN_JLIB_GRAPHICS_COLLADA_NAMESPACE

class JLIB_EXPORT base
    {
public:
    enum type { geometry };

    base( xml::tag *root=0 );
    void setMyRoot( xml::tag * );
    xml::tag *getMyRoot();

private:
    xml::tag *myRoot;
    };

END_JLIB_GRAPHICS_COLLADA_NAMESPACE

#endif
