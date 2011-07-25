#include "graphics/collada/graphics.collada.base.h"

BEGIN_JLIB_GRAPHICS_COLLADA_NAMESPACE

base::base( xml::tag *root ) : myRoot( root )
    {
    }

void base::setMyRoot( xml::tag *root )
    {
    myRoot = root;
    }

xml::tag *base::getMyRoot()
    {
    return myRoot;
    }
END_JLIB_GRAPHICS_COLLADA_NAMESPACE
