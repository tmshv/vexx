#include "multibbox.h"

#if 0
multiBBox::multiBBox( jLib::graphics::entity::entityG *p, unsigned int xMax, unsigned int yMax, unsigned int zMax ) : pointArray( p ),
        _xMax( xMax ), _yMax( yMax ), _zMax( zMax )
    {
    JFLOAT eSprConstant = 300;
    JFLOAT dSprConstant = 100;
    JFLOAT vSprConstant = 100;

    for( unsigned int z=0; z<=zMax; z++ )
        {
        for( unsigned int y=0; y<=yMax; y++ )
            {
            for( unsigned int x=0; x<=xMax; x++ )
                {
                _particles.push_back( cal::particle( 1, sseTriple( x, y, z ) ) );
                }
            }
        }

    for( unsigned int z=0; z<=zMax; z++ )
        {
        for( unsigned int y=0; y<=yMax; y++ )
            {
            for( unsigned int x=0; x<=xMax; x++ )
                {
                if( x < xMax )
                    {
                    _springs.push_back( doubleSpring( ) );
                    _springs.back().primary = &(particleAt( x, y, z ));
                    _springs.back().secondary = &(particleAt( x+1, y, z ));
                    _springs.back().restLength = 1;
                    _springs.back().constant = eSprConstant;
                    }
                if( y < yMax )
                    {
                    _springs.push_back( doubleSpring( ) );
                    _springs.back().primary = &(particleAt( x, y, z ));
                    _springs.back().secondary = &(particleAt( x, y+1, z ));
                    _springs.back().restLength = 1;
                    _springs.back().constant = eSprConstant;
                    }
                if( z < zMax )
                    {
                    _springs.push_back( doubleSpring( ) );
                    _springs.back().primary = &(particleAt( x, y, z ));
                    _springs.back().secondary = &(particleAt( x, y, z+1 ));
                    _springs.back().restLength = 1;
                    _springs.back().constant = eSprConstant;
                    }

                if( x < xMax && y < yMax )
                    {
                    _springs.push_back( doubleSpring( ) );
                    _springs.back().primary = &(particleAt( x, y, z ));
                    _springs.back().secondary = &(particleAt( x+1, y+1, z ));
                    _springs.back().restLength = sqrt( 2 );
                    _springs.back().constant = dSprConstant;
                    }

                if( x < xMax && z < zMax )
                    {
                    _springs.push_back( doubleSpring( ) );
                    _springs.back().primary = &(particleAt( x, y, z ));
                    _springs.back().secondary = &(particleAt( x+1, y, z+1 ));
                    _springs.back().restLength = sqrt( 2 );
                    _springs.back().constant = dSprConstant;
                    }

                if( y < yMax && z < zMax )
                    {
                    _springs.push_back( doubleSpring( ) );
                    _springs.back().primary = &(particleAt( x, y, z ));
                    _springs.back().secondary = &(particleAt( x, y+1, z+1 ));
                    _springs.back().restLength = sqrt( 2 );
                    _springs.back().constant = dSprConstant;
                    }

                if( x < xMax && y < yMax && z < zMax )
                    {
                    _springs.push_back( doubleSpring( ) );
                    _springs.back().primary = &(particleAt( x, y, z ));
                    _springs.back().secondary = &(particleAt( x+1, y+1, z+1 ));
                    _springs.back().restLength = sqrt( 3 );
                    _springs.back().constant = vSprConstant;

                    _springs.push_back( doubleSpring( ) );
                    _springs.back().primary = &(particleAt( x, y+1, z ));
                    _springs.back().secondary = &(particleAt( x+1, y, z+1 ));
                    _springs.back().restLength = sqrt( 3 );
                    _springs.back().constant = vSprConstant;

                    _springs.push_back( doubleSpring( ) );
                    _springs.back().primary = &(particleAt( x+1, y, z ));
                    _springs.back().secondary = &(particleAt( x, y+1, z+1 ));
                    _springs.back().restLength = sqrt( 3 );
                    _springs.back().constant = vSprConstant;

                    _springs.push_back( doubleSpring( ) );
                    _springs.back().primary = &(particleAt( x+1, y+1, z ));
                    _springs.back().secondary = &(particleAt( x, y, z+1 ));
                    _springs.back().restLength = sqrt( 3 );
                    _springs.back().constant = vSprConstant;
                    }
                }
            }
        }
    for( unsigned int z=0; z<zMax; z++ )
        {
        for( unsigned int y=0; y<yMax; y++ )
            {
            for( unsigned int x=0; x<xMax; x++ )
                {
                _boxes.push_back( bBox( particleAt( x, y, z ), particleAt( x+1, y, z ), particleAt( x+1, y, z+1 ), particleAt( x, y, z+1 ),
                                        particleAt( x, y+1, z ), particleAt( x+1, y+1, z ), particleAt( x+1, y+1, z+1 ), particleAt( x, y+1, z+1 ) ) );
                }
            }
        }
    }

jLib::vector <cal::particle> &multiBBox::particles()
    {
    return _particles;
    }

void multiBBox::update( JFLOAT t )
    {
        {
        FOREACH( _particles, p )
            {
            p->calculate( t );
            }
        }
        {
        FOREACH( _boxes, b )
            {
            b->update( t );
            }
        }
        {
        FOREACH( _springs, s )
            {
            s->update( t );
            }
        }
    }

void multiBBox::debugLines( jLib::graphics::primitives::lineCloud &lines )
    {
    lines.setLineArraySize( lines.sizeLines() + _springs.size() );
    lines.setPointArraySize( lines.sizePoints() + ( _springs.size() * 2 ) );

    FOREACH( _springs, s )
        {
        jLib::math::sseTriple force = s->secondary->position() - s->primary->position();
        JFLOAT ratio = force.length() / s->restLength;
        jLib::math::colour4 col( jLib::math::clamp( ratio, 0, 1 ), jLib::math::clamp( ratio-1, 0, 1 ), 0 );

        if( force.length() <= 0.1 * s->restLength || force.length() >= 2 * s->restLength )
            {
            col.b() = 1;
            }

        lines.addLine( lines.addPoint( s->primary->position(), col ),
                        lines.addPoint( s->secondary->position(), col ) );
        }
    }
#endif
