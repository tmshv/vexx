#include "graphics/primitives/graphics.primitives.polygonMesh.h"

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

polygonMesh::polygonMesh( triangleArray *slave, bool destroySlave ) : points( &(slave->_pointData) ),
        normals( &(slave->_pointData) ), textures( &(slave->_pointData) ), colours( &(slave->_pointData) ),
        _slave( slave ), _destroySlave( destroySlave )
    {
    jAssert( _slave );
    }

polygonMesh::~polygonMesh( )
    {
    if( _destroySlave )
        {
        delete _slave;
        }
    }

/*unsigned int polygonMesh::addPolygon( unsigned int n )
    {
    struct _polygonData temp;
    _polygons.push_back( temp );
    if( n != 0 )
        {
        resizePolygon( _polygons.size() - 1, n );
        }
    return _polygons.size() - 1;
    }*/

void polygonMesh::resizePolygonArray( unsigned int n )
    {
    _polygons.resize( n );
    }

unsigned int polygonMesh::sizePolygonArray( ) const
    {
    return _polygons.size();
    }

void polygonMesh::clearPolygonArray( )
    {
    _polygons.clear();
    }

void polygonMesh::clearPolygon( unsigned int x )
    {
    jAssert( x < _polygons.size() );
    _polygons[x].points.clear();
    }

void polygonMesh::resizePolygon( unsigned int x, unsigned int n )
    {
    jAssert( x < _polygons.size() );
    _polygons[x].points.resize( n );
    }

unsigned int polygonMesh::sizePolygon( unsigned int x ) const
    {
    jAssert( x < _polygons.size() );
    return _polygons[x].points.size();
    }

unsigned int polygonMesh::getPolygonVertexPoint( unsigned int x, unsigned int y ) const
    {
    jAssert( x < _polygons.size() );
    jAssert( y < _polygons[x].points.size() );
    return _polygons[x].points[y].point;
    }

unsigned int polygonMesh::getPolygonVertexNormal( unsigned int x, unsigned int y ) const
    {
    jAssert( x < _polygons.size() );
    jAssert( y < _polygons[x].points.size() );
    return _polygons[x].points[y].normal;
    }

unsigned int polygonMesh::getPolygonVertexTexture( unsigned int x, unsigned int y ) const
    {
    jAssert( x < _polygons.size() );
    jAssert( y < _polygons[x].points.size() );
    return _polygons[x].points[y].texture;
    }

unsigned int polygonMesh::getPolygonVertexColour( unsigned int x, unsigned int y ) const
    {
    jAssert( x < _polygons.size() );
    jAssert( y < _polygons[x].points.size() );
    return _polygons[x].points[y].colour;
    }

void polygonMesh::setPolygonVertexPoint( unsigned int x, unsigned int y, unsigned int z )
    {
    jAssert( x < _polygons.size() );
    jAssert( y < _polygons[x].points.size() );
    _polygons[x].points[y].point = z;
    }

void polygonMesh::setPolygonVertexNormal( unsigned int x, unsigned int y, unsigned int z )
    {
    jAssert( x < _polygons.size() );
    jAssert( y < _polygons[x].points.size() );
    _polygons[x].points[y].normal = z;
    }

void polygonMesh::setPolygonVertexTexture( unsigned int x, unsigned int y, unsigned int z )
    {
    jAssert( x < _polygons.size() );
    jAssert( y < _polygons[x].points.size() );
    _polygons[x].points[y].texture = z;
    }

void polygonMesh::setPolygonVertexColour( unsigned int x, unsigned int y, unsigned int z )
    {
    jAssert( x < _polygons.size() );
    jAssert( y < _polygons[x].points.size() );
    _polygons[x].points[y].colour = z;
    }

void polygonMesh::rebuild()
    {
    // Clear the slate
    _slave->clear();

    // How many triangles do we have in total?
    unsigned int pointTotal = 0;
    unsigned int usedPoints = 0;
    unsigned int triIndex = 0;
    for( unsigned int poly=0; poly<sizePolygonArray(); poly++ )
        {
        // The sum below finds the numer of points in a polygon if each triangle
        // is specified individually, so triangle = 3 points, minus 2 = 1, times 3
        // equals 3. quad minus 2 = 2, times 3 = 6... etc...
        pointTotal += 3 * ( sizePolygon( poly ) - 2 );
        }
    // make sure we have the required memory in the slave.
    _slave->setPointArraySize( pointTotal );

    _slave->_indexData.resize( ( pointTotal / 3 ) );
    triangleArrayIndexPrim *slaveIndexData = &(_slave->_indexData[0]);
    for( unsigned int poly=0; poly<sizePolygonArray(); poly++ )
        {
        vector <struct _vertexData> &pointsNow=_polygons[poly].points;
        unsigned int trianglesInPoly = pointsNow.size() - 2;
        for( unsigned int tri=0; tri<trianglesInPoly; tri++ )
            {
            // so at this point we will inspect every theoretical point in the mesh, and need to
            // create every real point in the mesh.
            // these pointers represent the vertexes in the current triangle
            struct _vertexData *vertPts[3] = { &(pointsNow[0]), &(pointsNow[tri+1]), &(pointsNow[tri+2]) };

            slaveIndexData[triIndex].a = usedPoints;
            slaveIndexData[triIndex].b = usedPoints+1;
            slaveIndexData[triIndex].c = usedPoints+2;

            for( unsigned int pt=0; pt<3; pt++ )
                {
                jAssert( vertPts[pt]->point < points.size() );
                jAssert( vertPts[pt]->normal < normals.size() );
                jAssert( vertPts[pt]->texture < textures.size() );
                jAssert( vertPts[pt]->colour < colours.size() );

                points._data[vertPts[pt]->point].slaves.push_back( usedPoints );
                normals._data[vertPts[pt]->normal].slaves.push_back( usedPoints );
                textures._data[vertPts[pt]->texture].slaves.push_back( usedPoints );
                colours._data[vertPts[pt]->colour].slaves.push_back( usedPoints );
                ++usedPoints;
                }
            triIndex++;
            }
        }
    setAll();
    }

void polygonMesh::setAll()
    {
    for( unsigned int x=0; x<points.size(); x++ )
        {
        points.informSlave(x);
        }
    for( unsigned int x=0; x<normals.size(); x++ )
        {
        normals.informSlave(x);
        }
    for( unsigned int x=0; x<textures.size(); x++ )
        {
        textures.informSlave(x);
        }
    for( unsigned int x=0; x<colours.size(); x++ )
        {
        colours.informSlave(x);
        }
    }

triangleArray *polygonMesh::getTriangleArray()
    {
    return _slave;
    }

const triangleArray *polygonMesh::getTriangleArray() const
    {
    return _slave;
    }

void polygonMesh::setTriangleArray( triangleArray *slave )
    {
    _slave = slave;
    jAssert( _slave );
    }

/*void polygonMesh::updateInternalIDs( unsigned int min, unsigned int sub )
    {
    for( unsigned int poly=0; poly<_polygons.size(); poly++ )
        {
        for( unsigned int vert=0; vert<_polygons[poly].points.size(); vert++ )
            {
            if( _polygons[poly].points[vert].arrayID > min )
                {
                _polygons[poly].points[vert].arrayID -= sub;
                }
            }
        }
    }*/

bool insertIfUnique( unsigned int a, unsigned int b, jLib::list <jLib::pair<unsigned int, unsigned int> > &li )
    {
    jLib::pair<unsigned int, unsigned int> p;
    if( a < b )
        {
        p.first = a;
        p.second = b;
        }
    else
        {
        p.first = b;
        p.second = a;
        }
    FOREACH( li, l )
        {
        if( l->first == p.first && l->second == p.second )
            {
            return FALSE;
            }
        }
    li.push_back( p );
    return TRUE;
    }

jLib::list <jLib::pair<unsigned int, unsigned int> > polygonMesh::listEdges() const
    {
    jLib::list <jLib::pair<unsigned int, unsigned int> > ret;
    FOREACHCONST( _polygons, p )
        {
        for( unsigned int x=0; x<(p->points.size()-1); x++ )
            {
            insertIfUnique( p->points[x].point, p->points[x+1].point, ret );
            }
        }
    return ret;
    }

jLib::list <jLib::pair<unsigned int, unsigned int> > polygonMesh::listTriangleEdges() const
    {
    jLib::list <jLib::pair<unsigned int, unsigned int> > ret;

    FOREACHCONST( _polygons, p )
        {
        for( unsigned int x=0; x<(p->points.size()-1); x++ )
            {
            insertIfUnique( p->points[x].point, p->points[x+1].point, ret );
            insertIfUnique( p->points[0].point, p->points[x+1].point, ret );
            if( x < (p->points.size()-2) )
                {
                insertIfUnique( p->points[0].point, p->points[x+2].point, ret );
                }
            }
        }
    return ret;
    }

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE
