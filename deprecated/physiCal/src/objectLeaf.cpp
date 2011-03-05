#include "objectLeaf.h"

BEGIN_PHYSICAL_NAMESPACE

objectLeaf::objectLeaf( octree <objectLeaf> *parent ) : _parent( parent )
    {
    }

void objectLeaf::setup( QList <objectFace> &faces )
    {
    //empty bounds
    _bounds.clear();
    FOREACH( faces, f )
        {
        // refresh the bounding cube of each face in this leaf
        f->refreshBoundingCube();
        // refresh my bounding cube from the face
        _bounds.suggest( f->bounds() );
        // add the passed in faces to my face list
        _faces.push_back( &(*f) );
        }

    // if i hold more than 8 faces ( minimum worthwile amount to subdivide ), and its not a tiny box, to prevent recursion
    if( faces.size() > 8 &&
        ( ( _bounds.size().x() > 2 ) && ( _bounds.size().y() > 2 ) && ( _bounds.size().z() > 2 ) ) )
        {
        // subdivide
        _parent->divide();

        // and setup the sub boxes
        setupBox( 0, 0, 0 );
        setupBox( 1, 0, 0 );
        setupBox( 0, 1, 0 );
        setupBox( 1, 1, 0 );
        setupBox( 0, 0, 1 );
        setupBox( 1, 0, 1 );
        setupBox( 0, 1, 1 );
        setupBox( 1, 1, 1 );
        }
    }

void objectLeaf::setupBox( unsigned int x, unsigned int y, unsigned int z )
    {
    // find my position vector
    position3 pos( x, y, z );
    // create min and max vectors
    position3 min( position3( 0.0 ) + ( vector3( 0.5 ) * pos ) );
    position3 max( position3( 0.5 ) + ( vector3( 0.5 ) * pos ) );

    // find the boxes sub bounding box from percentages of this bounding box
    boundingCube _subBox = boundingCube::fromCuboid( _bounds, min, max );


    // distribute this classes faces to thsi box
    QList <objectFace> _subFaces;
    _subFaces.clear();
    FOREACH( _faces, f )
        {
        if( (*f)->bounds().test( _subBox ) )
            {
            _subFaces.push_back( **f );
            }
        }

    // setup child
    _parent->at( x, y, z ).contents().setup( _subFaces );
    }

void objectLeaf::refreshBoundingCube()
    {
    // if i have children
    if( _parent->mode() == octree<objectLeaf>::Branch )
        {
        // do the sub children
        _bounds.clear();
        for( unsigned int x=0; x<8; x++ )
            {
            _parent->at( x ).contents().refreshBoundingCube();
            // then update me too
            _bounds.suggest( _parent->at( x ).contents()._bounds );
            }
        }
    else
        {
        // else jsut suggest the faces to me
        _bounds.clear();
        FOREACH( _faces, f )
            {
            _bounds.suggest( (*f)->bounds() );
            }
        }
    }

END_PHYSICAL_NAMESPACE
