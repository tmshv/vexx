#ifndef JLIB_GRAPHICS_PRIMITIVES_TRIANGLEOBJECTDATACACHE_H
#define JLIB_GRAPHICS_PRIMITIVES_TRIANGLEOBJECTDATACACHE_H

#include "graphics/primitives/graphics.primitives.types.h"

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE


struct triangleArrayPointPrim;

template <typename T> class polygonMeshDataCache
    {
public:
    polygonMeshDataCache( vector <triangleArrayPointPrim> *slaveData )
        {
        setSlave( slaveData );
        }
    virtual ~polygonMeshDataCache()
        {
        }
    void setSlave( vector <triangleArrayPointPrim> *slaveData )
        {
        _slaveData = slaveData;
        }
    T get( unsigned int x ) const
        {
        jAssert( x < size() );
        return _data[x].data;
        }
    void set( T t, unsigned int x )
        {
        jAssert( x < size() );
        _data[x].data = t;
        informSlave( x );
        }
    void set( const vector <T> &vec )
        {
        clear();
        resize( vec.size() );
        _dataCache temp;
        for( unsigned int x=0; x<vec.size(); x++ )
            {
            temp.data = vec[x];
            _data.push_back( temp );
            informSlave( x );
            }
        }
    void remove( unsigned int x )
        {
        jAssert( x < size() );
        _data.erase( _data.begin() + x );
        }
    unsigned int add( T t )
        {
        _dataCache temp;
        temp.data = t;
        _data.push_back( temp );
        return _data.size() - 1;
        }
    unsigned int size() const
        {
        return _data.size();
        }
    void resize( unsigned int n )
        {
        _data.reserve( n );
        }
    void clear()
        {
        _data.clear();
        }
protected:
    virtual void informSlave( unsigned int id )=0;
    struct _dataCache
        {
        T data;
        vector <unsigned int> slaves;
        };
    vector <_dataCache> _data;
    vector <triangleArrayPointPrim> *_slaveData;

    friend class polygonMesh;
    };

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

#endif
