#include "XPrimitives.h"
#include "XAbstractRenderer.h"
#include "XAbstractGeometryCache.h"

XLocator::~XLocator()
    {
    delete cache;
    }

void XLocator::initiate()
    {
    cache = renderer()->getGeometryCache();

    cache->setLines( XList<uint>() << 0 << 1 << 2 << 3 << 4 << 5 );

    cache->setVertices( XList<XVector3D>()
                      << XVector3D( -1, 0, 0 )
                      << XVector3D( 1, 0, 0 )
                      << XVector3D( 0, -1, 0 )
                      << XVector3D( 0, 1, 0 )
                      << XVector3D( 0, 0, -1 )
                      << XVector3D( 0, 0, 1 ) );
    }

void XLocator::render()
    {
    renderer()->drawGeomtreyCache( cache );
    }
