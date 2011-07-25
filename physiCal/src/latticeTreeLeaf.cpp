#include "latticeTreeLeaf.h"

BEGIN_PHYSICAL_NAMESPACE

latticeTreeLeaf::latticeTreeLeaf( octree <latticeTreeLeaf> *p ) : _parent( p )
    {
    }

void latticeTreeLeaf::distribute( QList <particle> &parts )
    {
    FOREACH( parts, p )
        {
        _bounds.suggest( p->position() );
        }

    calculateLeafBoundsFromParent();
    }

void latticeTreeLeaf::calculateLeafBoundsFromParent()
    {
    if( _parent->mode() == octree<latticeTreeLeaf>::Branch )
        {
        for( unsigned int z=0; z<2; z++ )
            {
            for( unsigned int y=0; y<2; y++ )
                {
                for( unsigned int x=0; x<2; x++ )
                    {
                    //_parent->at( x, y, z ).contents()._bounds.setOctreeLeafFromParent( _bounds, x, y, z );
                    /*if( _parent->at( x, y, z ).mode() == octree<latticeTreeLeaf>::Branch )
                        {
                        _parent->at( x, y, z ).contents().calculateLeafBoundsFromParent();
                        }*/
                    }
                }
            }
        }
    }

END_PHYSICAL_NAMESPACE
