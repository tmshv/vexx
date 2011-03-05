#ifndef OCTREE_H
#define OCTREE_H

#include "physiCalGlobal.h"

BEGIN_PHYSICAL_NAMESPACE

//! An octree container class
template <typename T> class PHYSICAL_EXPORT octree
    {
public:
    //! An enum to describe what a leafs job is
    enum divisionMode { Branch, Leaf };
    //! this octree leafs mode
    jROProperty( divisionMode, mode );
    //! this leafs parent ( 0 for root )
    jROProperty( octree <T> *, parent );

public:
    //! create an octree with parent p
    octree( octree <T> *p=0 ) : _mode( Leaf ), _parent( p ), _contents( this )
        {
        }
    // copy a leaf
    octree( const octree &in ) : _contents( in._contents )
        {
        _mode = in._mode;
        if( _mode == Branch )
            {
            for( unsigned int x=0; x<8; x++ )
                {
                _stem[x] = new octree <T>( in._stem[x] );
                }
            }
        }
    //! dstroys this leaf and its children
    ~octree()
        {
        undivide();
        }
    //! divide this leaf inRes times, creating 2^inRes leaves
    void divide( unsigned int inRes )
        {
        if( inRes > 0 )
            {
            divide();
            for( unsigned int x=0; x<8; x++ )
                {
                _stem[x]->divide( inRes-1 );
                }
            }
        }
    //! divide this leaf, no effect if already divided
    void divide()
        {
        if( _mode == Leaf )
            {
            _mode = Branch;
            for( unsigned int x=0; x<8; x++ )
                {
                _stem[x] = new octree <T>( this );
                }
            }
        }
    //! undivide this leaf, no effect if not divided
    void undivide()
        {
        if( _mode == Branch )
            {
            _mode = Leaf;
            for( unsigned int x=0; x<8; x++ )
                {
                delete _stem[x];
                }
            }
        }
    //! get the contents of this leaf
    const T &contents() const
        {
        return _contents;
        }
    //! get the contents of this leaf
    T &contents()
        {
        return _contents;
        }
    //! get the child of this leaf at (x,y,z)
    const octree &at( unsigned int x, unsigned int y, unsigned int z ) const
        {
        return at( x + y * 2 + z * 4 );
        }
    //! get the child of this leaf at (x,y,z)
    octree &at( unsigned int x, unsigned int y, unsigned int z )
        {
        return at( x + y * 2 + z * 4 );
        }
    //! get the child at index in, x should be between 0 and 7
    const octree &at( unsigned int in ) const
        {
        jAssert( in < 8 );
        jAssert( _mode == Branch );
        return *(_stem[in]);
        }
    //! get the child at index in, x should be between 0 and 7
    octree &at( unsigned int in )
        {
        jAssert( in < 8 );
        jAssert( _mode == Branch );
        return *(_stem[in]);
        }
    //! find the division levels below this leaf
    unsigned int levels( ) const
        {
        if( _parent )
            {
            return _parent->levels() + 1;
            }
        return 0;
        }
    //! get a list of leaves below this leaf
    QList <octree <T> *> leaves()
        {
        QList <octree <T> *> ret;
        leaves( ret );
        return ret;
        }
    //! get a list of leaves below this leaf into the list le
    void leaves( QList <octree <T> *> &le )
        {
        if( _mode = Branch )
            {
            for( unsigned int x=0; x<8; x++ )
                {
                leaves( le );
                }
            }
        else
            {
            le.push_back( this );
            }
        }

private:
    T _contents;
    octree <T> *_stem[8];
    };

END_PHYSICAL_NAMESPACE

#endif
