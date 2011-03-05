#include "lattice.h"

BEGIN_PHYSICAL_NAMESPACE

lattice::lattice(  const springMaterial &mat, const graphics::primitives::triangleArray *arr,
        const position3 &pos, unsigned int maxRes )
        : _material( &mat )
    {
    for( unsigned int x=0; x<arr->sizeTriangles(); x++ )
        {
        _faces.push_back( face( pos + position3( arr->getPointPosition( arr->getTriangleA( x ) ) ),
                                pos + position3( arr->getPointPosition( arr->getTriangleB( x ) ) ),
                                pos + position3( arr->getPointPosition( arr->getTriangleC( x ) ) ) ) );
        }

    FOREACH( _faces, f )
        {
        _bounds.suggest( f->pointA() );
        _bounds.suggest( f->pointB() );
        _bounds.suggest( f->pointC() );
        }

    vector3 size = _bounds.size();
    int xRes, yRes, zRes;
    if( size.x() > size.y() && size.x() > size.z() )
        {
        xRes = maxRes;
        yRes = (int)(size.y() * ( (float)maxRes / size.x() ));
        zRes = (int)(size.z() * ( (float)maxRes / size.x() ));
        }
    else if( size.y() > size.z() )
        {
        xRes = (int)(size.x() * ( (float)maxRes / size.y() ));
        yRes = maxRes;
        zRes = (int)(size.z() * ( (float)maxRes / size.y() ));
        }
    else
        {
        xRes = (int)(size.x() * ( (float)maxRes / size.z() ));
        yRes = (int)(size.y() * ( (float)maxRes / size.z() ));
        zRes = maxRes;
        }
    if( xRes < 1 )
        {
        xRes = 1;
        }
    if( yRes < 1 )
        {
        yRes = 1;
        }
    if( zRes < 1 )
        {
        zRes = 1;
        }

    _xMax = xRes;
    _yMax = yRes;
    _zMax = zRes;

    initFromBoundingCube();
    linkFacesToParticles();
    }

void lattice::linkFacesToParticles()
    {
    for( unsigned int z=0; z<_zMax; z++ )
        {
        for( unsigned int y=0; y<_yMax; y++ )
            {
            for( unsigned int x=0; x<_xMax; x++ )
                {
                boundingCube curBox( particleAt( x, y, z ).position(), particleAt( x+1, y+1, z+1 ).position() );
                FOREACH( _faces, f )
                    {
                    if( curBox.test( f->pointA() ) )
                        {
                        f->boxA().set( particleAt( x, y, z ), particleAt( x+1, y, z ),
                                       particleAt( x+1, y+1, z ), particleAt( x, y+1, z ),
                                       particleAt( x, y, z+1 ), particleAt( x+1, y, z+1 ),
                                       particleAt( x+1, y+1, z+1 ), particleAt( x, y+1, z+1 ) );
                        }
                    if( curBox.test( f->pointB() ) )
                        {
                        f->boxB().set( particleAt( x, y, z ), particleAt( x+1, y, z ),
                                       particleAt( x+1, y+1, z ), particleAt( x, y+1, z ),
                                       particleAt( x, y, z+1 ), particleAt( x+1, y, z+1 ),
                                       particleAt( x+1, y+1, z+1 ), particleAt( x, y+1, z+1 ) );
                        }
                    if( curBox.test( f->pointC() ) )
                        {
                        f->boxC().set( particleAt( x, y, z ), particleAt( x+1, y, z ),
                                       particleAt( x+1, y+1, z ), particleAt( x, y+1, z ),
                                       particleAt( x, y, z+1 ), particleAt( x+1, y, z+1 ),
                                       particleAt( x+1, y+1, z+1 ), particleAt( x, y+1, z+1 ) );
                        }
                    }
                }
            }
        }

    FOREACH( _faces, f )
        {
        f->makeParametric();
        }
    }

void lattice::initFromBoundingCube()
    {
    vector3 oneBoxSize = vector3( _bounds.size() / vector3( _xMax, _yMax, _zMax ) );

    pReal perParticleMass = ( material().density() * _bounds.volume() ) / ( _xMax * _yMax * _zMax );

    if( material().density() == jLib::floatLimits::infinity() || _bounds.volume() == 0 )
        {
        perParticleMass = jLib::floatLimits::infinity();
        }

    for( unsigned int z=0; z<=_zMax; z++ )
        {
        for( unsigned int y=0; y<=_yMax; y++ )
            {
            for( unsigned int x=0; x<=_xMax; x++ )
                {
                _particles.push_back(
                    particle(
                        perParticleMass,
                        vector3( x*oneBoxSize.x(), y*oneBoxSize.y(), z*oneBoxSize.z() ) + _bounds.minimum(),
                        0.0 ) );
                }
            }
        }
    initSpringsFromParticles();
    }

void lattice::initSpringsFromParticles()
    {
    for( unsigned int z=0; z<=_zMax; z++ )
        {
        for( unsigned int y=0; y<=_yMax; y++ )
            {
            for( unsigned int x=0; x<=_xMax; x++ )
                {
                if( x < _xMax )
                    {
                    _springs.push_back( spring( particleAt( x, y, z ), particleAt( x+1, y, z ), material() ) );
                    }
                if( y < _yMax )
                    {
                    _springs.push_back( spring( particleAt( x, y, z ), particleAt( x, y+1, z ), material() ) );
                    }
                if( z < _zMax )
                    {
                    _springs.push_back( spring( particleAt( x, y, z ), particleAt( x, y, z+1 ), material() ) );
                    }

                if( x < _xMax && y < _yMax )
                    {
                    _springs.push_back( spring( particleAt( x, y, z ), particleAt( x+1, y+1, z ), material(), springMaterial::Diagonal ) );
                    _springs.push_back( spring( particleAt( x, y+1, z ), particleAt( x+1, y, z ), material(), springMaterial::Diagonal ) );
                    }

                if( x < _xMax && z < _zMax )
                    {
                    _springs.push_back( spring( particleAt( x, y, z ), particleAt( x+1, y, z+1 ), material(), springMaterial::Diagonal ) );
                    _springs.push_back( spring( particleAt( x, y, z+1 ), particleAt( x+1, y, z ), material(), springMaterial::Diagonal ) );
                    }

                if( y < _yMax && z < _zMax )
                    {
                    _springs.push_back( spring( particleAt( x, y, z ), particleAt( x, y+1, z+1 ), material(), springMaterial::Diagonal ) );
                    _springs.push_back( spring( particleAt( x, y+1, z ), particleAt( x, y, z+1 ), material(), springMaterial::Diagonal ) );
                    }

                if( x < _xMax && y < _yMax && z < _zMax )
                    {
                    _springs.push_back( spring( particleAt( x, y, z ), particleAt( x+1, y+1, z+1 ), material(), springMaterial::Volume ) );

                    _springs.push_back( spring( particleAt( x, y+1, z ), particleAt( x+1, y, z+1 ), material(), springMaterial::Volume ) );

                    _springs.push_back( spring( particleAt( x+1, y, z ), particleAt( x, y+1, z+1 ), material(), springMaterial::Volume ) );

                    _springs.push_back( spring( particleAt( x+1, y+1, z ), particleAt( x, y, z+1 ), material(), springMaterial::Volume ) );
                    }
                }
            }
        }
    }

lattice::lattice( const lattice &in ) : _material( in._material ), _faces( in._faces ),
        _xMax( in._xMax ), _yMax( in._yMax ), _zMax( in._zMax ), _bounds( in._bounds )
    {
    FOREACH( _faces, f )
        {
        f->mode( face::Absolute );
        }
    initFromBoundingCube();
    linkFacesToParticles();
    }


const QList <particle> &lattice::particles() const
    {
    return _particles;
    }

QList <particle> &lattice::particles()
    {
    return _particles;
    }

QList <face> &lattice::faces()
    {
    return _faces;
    }

const QList <face> &lattice::faces() const
    {
    return _faces;
    }

void lattice::update( pReal t )
    {
        {
        //#pragma omp parallel for
        for( int x=0; x<(int)_springs.size(); x++ )
            {
            _springs[x].update( t );
            }
        }
        {
        //#pragma omp parallel for
        for( int x=0; x<(int)_particles.size(); x++ )
            {
            _particles[x].calculate( t );
            }
        }
    }

void lattice::refreshBoundingCube()
    {
    _bounds.clear();
    _octreeBoundingBoxesNeedRefresh = TRUE;

        {
        FOREACH( _particles, p )
            {
            _bounds.suggest( p->position() );
            }
        }
        {
        FOREACH( _faces, f )
            {
            f->refreshBoundingCube();
            }
        }
    }

const boundingCube &lattice::bounds() const
    {
    return _bounds;
    }

void lattice::collisions( lattice &in, collisionArray & )
    {
    bool self = &in == this;
    if( in._bounds.test( this->_bounds ) && in._faces.size() && _faces.size() )
        {
        for( int x=0; x<(int)_faces.size(); x++ )
            {
            for( int y=0; y<(int)in._faces.size(); y++ )
                {
                if( x != y || !self )
                    {
                    vector3 disp;

                    if( collision( _faces[x], in._faces[y], disp ) )
                        {
                        //arr.add( _faces[x], in._faces[y], material().restitution() * in.material().restitution(), disp.length(), disp.normalise() );
                        }
                    }
                }
            }
        }
    }

void lattice::debugTriangles( jLib::graphics::primitives::triangleCloud &tris )
    {
    tris.setTriangleArraySize( tris.sizeTriangles() + _faces.size() );
    tris.setPointArraySize( tris.sizePoints() + ( 3 *_faces.size() ) );

    vector3 n( 0.0 );
    jLib::math::vector2 t( 0.0 );
    jLib::math::colour4 c( 0.0, 1.0, 0.0, 0.2 );

    FOREACH( _faces, f )
        {
        tris.addTriangle( tris.addPoint( f->pointA(), n, t, c ),
                          tris.addPoint( f->pointB(), n, t, c ),
                          tris.addPoint( f->pointC(), n, t, c ));

        }
    }

void lattice::debugNormals( jLib::graphics::primitives::lineCloud &lines )
    {
    lines.setLineArraySize( lines.sizeLines() + _faces.size() );
    lines.setPointArraySize( lines.sizePoints() + ( _faces.size() * 2 ) );

    FOREACH( _faces, f )
        {
        lines.addLine( f->averagePosition(), f->averagePosition() + vector3( f->pointB() - f->pointA() ).cross( f->pointC() - f->pointA() ).scaleTo( 0.2 ) );
        }
    }

void lattice::debugLines( jLib::graphics::primitives::lineCloud &lines )
    {
    lines.setLineArraySize( lines.sizeLines() + _springs.size() );
    lines.setPointArraySize( lines.sizePoints() + ( _springs.size() * 2 ) );

    FOREACH( _springs, s )
        {
        vector3 force = s->secondary().position() - s->primary().position();
        pReal ratio = force.length() / s->restLength();
        jLib::math::colour4 col( jLib::math::clamp( ratio, 0, 1 ), jLib::math::clamp( ratio-1, 0, 1 ), 0 );

        if( force.length() <= 0.1 * s->restLength() || force.length() >= 2 * s->restLength() )
            {
            col.b() = 1;
            }

        lines.addLine( lines.addPoint( s->primary().position(), col ),
                        lines.addPoint( s->secondary().position(), col ) );
        }
    }

void lattice::debugPoints( jLib::graphics::primitives::pointCloud &points )
    {
    points.setPointArraySize( points.sizePoints() + _particles.size() );

    FOREACH( _particles, s )
        {
        points.addPoint( s->position(), jLib::math::colour4::YELLOW );
        }
    }


lattice lattice::ground( const springMaterial &mat, pReal x, pReal y )
    {
    x /= 2;
    y /= 2;
    jLib::graphics::primitives::triangleCloud cloud( 0 );
    cloud.addQuad( cloud.addPoint( position3( -x, 0, -y ), 0.0, 0.0, 0.0 ),
                    cloud.addPoint( position3( -x, 0, y ), 0.0, 0.0, 0.0 ),
                    cloud.addPoint( position3( x, 0, y ), 0.0, 0.0, 0.0 ),
                    cloud.addPoint( position3( x, 0, -y ), 0.0, 0.0, 0.0 ) );
    return lattice( mat, &cloud, 0.0, 1 );
    }

END_PHYSICAL_NAMESPACE
