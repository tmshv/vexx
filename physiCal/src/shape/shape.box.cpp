#include "shape/shape.box.h"

BEGIN_PHYSICAL_NAMESPACE

box::box( pVector s, pVector pos ) : position( pos ), size( s )
    {
    }

box::~box( )
    {
    }

pReal box::shortestDistance( pVector )
    {
    return floatLimits::infinity();
    }

pReal box::shortestDistance( line )
    {
    return floatLimits::infinity();
    }

END_PHYSICAL_NAMESPACE
