#include "boundingBox.h"

boundingBox::boundingBox( jLib::graphics::entity::entityG *p, const jLib::math::triple &min, const jLib::math::triple &max ) : lineCloud( p )
    {
    rebuild( min, max );
    }

void boundingBox::rebuild( const jLib::math::triple & min, const jLib::math::triple &max )
    {
    clearLines();
    clearPoints();
    unsigned int points[8] = { addPoint( min, jLib::math::colour4::WHITE ),
        addPoint( jLib::math::triple( max.x(), min.y(), min.z() ), jLib::math::colour4::WHITE ),
        addPoint( jLib::math::triple( max.x(), min.y(), max.z() ), jLib::math::colour4::WHITE ),
        addPoint( jLib::math::triple( min.x(), min.y(), max.z() ), jLib::math::colour4::WHITE ),
        addPoint( jLib::math::triple( min.x(), max.y(), min.z() ), jLib::math::colour4::WHITE ),
        addPoint( jLib::math::triple( max.x(), max.y(), min.z() ), jLib::math::colour4::WHITE ),
        addPoint( max, jLib::math::colour4::WHITE ),
        addPoint( jLib::math::triple( min.x(), max.y(), max.z() ), jLib::math::colour4::WHITE ) };

    unsigned int bottom[4] = { points[0], points[1], points[2], points[3] };
    unsigned int top[4] = { points[4], points[5], points[6], points[7] };
    addLineLoop( bottom, 4 );
    addLineLoop( top, 4 );
    addLine( 0, 4 );
    addLine( 1, 5 );
    addLine( 2, 6 );
    addLine( 3, 7 );
    }
