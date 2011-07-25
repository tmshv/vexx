#include "graphics/entity/graphics.entity.bBox.h"

BEGIN_JLIB_GRAPHICS_ENTITY_NAMESPACE

bBox::bBox( )
    {
    }

void bBox::clear()
    {
    shape::box::position.x() = floatLimits::infinity();
    shape::box::position.y() = floatLimits::infinity();
    shape::box::position.z() = floatLimits::infinity();
    shape::box::size.x() = 0;
    shape::box::size.y() = 0;
    shape::box::size.z() = 0;
    }

void bBox::suggest( math::triple newEx )
    {
    for( unsigned int x=0; x<2; x++ )
        {
        if( newEx[x] < shape::box::position[x] )
            {
            shape::box::position[x] = newEx[x];
            }
        }
    for( unsigned int x=0; x<2; x++ )
        {
        if( newEx[x] > shape::box::position[x]+shape::box::size[x] )
            {
            shape::box::size[x] = newEx[x];
            }
        }
    }

bool bBox::isInside( math::triple )
    {
    jDebug<<"This function will be a replicant of the box shape class"<<endl;
    return 0;
    }

bool bBox::isInside( shape::line )
    {
    jDebug<<"This function will be a replicant of the box shape class"<<endl;
    return 0;
    }

bool bBox::isInside( const bBox & )
    {
    jDebug<<"This function will be a replicant of the box shape class"<<endl;
    return 0;
    }

END_JLIB_GRAPHICS_ENTITY_NAMESPACE
