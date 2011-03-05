#include "jLib.global.h"
#include "jLib.vector.h"
#include <QDebug>
#include <QHBoxLayout>


#ifdef JLIB_MEMORY_DEBUG
class memTrack
    {
public:
    enum type { SINGLE, ARRAY };
    memTrack() : _maxAlloc( 0 ), _allocID( 0 )
        {
        }
    ~memTrack()
        {
        jLib::vector <memTrackOp> tempCA = _cA;
        unsigned int tempMaxAlloc = _maxAlloc;
        qDebug()<<"Allocations not deleted after application quit: "<<tempCA.size();
        qDebug()<<"Maximum allocations throughout the application: "<<tempMaxAlloc;
        FOREACH( tempCA, mem )
            {
            QString t = "single";
            if( mem->t == ARRAY )
                {
                t = "array";
                }
            qDebug()<<"Alloc "<<mem->id<<" / "<<_maxAlloc<<": "<<mem->size<<" bytes, for a "<<t;
            }*/
        }
    void add( void *ptr, size_t size, type t )
        {
        memTrackOp temp;

        temp.ptr = ptr;
        temp.size = size;
        temp.t = t;
        temp.id = _allocID++;
        _cA.push_back( temp );
        if( _cA.size() > _maxAlloc )
            {
            _maxAlloc = _cA.size();
            }
        }
    void *back()
        {
        return _cA.back().ptr;
        }
    void remove( void *ptr )
        {
        for( unsigned int x=0; x<_cA.size(); x++ )
            {
            if( _cA.at(x).ptr == ptr )
                {
                _cA.erase( _cA.begin() + x );
                }
            }
        }
private:
    struct memTrackOp
        {
        void *ptr;
        size_t size;
        type t;
        unsigned int id;
        };
    unsigned int _maxAlloc;
    jLib::vector <memTrackOp> _cA;
    unsigned int _allocID;
    };

static memTrack memory;

void *operator new( size_t size )
    {
    static bool inOp = FALSE;

    if( inOp )
        {
        return malloc( size );
        }
    inOp = TRUE;
    memory.add( malloc( size ), size, memTrack::SINGLE );
    inOp = FALSE;
    return memory.back();
    }

void *operator new[]( size_t size )
    {
    static bool inOp = FALSE;

    if( inOp )
        {
        return malloc( size );
        }
    inOp = TRUE;
    memory.add( malloc( size ), size, memTrack::ARRAY );
    inOp = FALSE;
    return memory.back();
    }

void operator delete( void *ptr )
    {
    static bool inOp = FALSE;

    if( inOp )
        {
        free( ptr );
        return;
        }
    inOp = TRUE;
    memory.remove( ptr );
    inOp = FALSE;
    free( ptr );
    }

void operator delete[]( void *ptr )
    {
    static bool inOp = FALSE;

    if( inOp )
        {
        free( ptr );
        return;
        }
    inOp = TRUE;
    memory.remove( ptr );
    inOp = FALSE;
    free( ptr );
    }

#else

void *operator new( size_t size )
    {
    return malloc( size );
    }

void *operator new[]( size_t size )
    {
    return malloc( size );
    }

void operator delete( void *ptr )
    {
    free( ptr );
    }

void operator delete[]( void *ptr )
    {
    free( ptr );
    }

#endif
