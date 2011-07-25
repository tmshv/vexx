#ifndef JLIB_VECTOR_H
#define JLIB_VECTOR_H

#include "jLib.global.h"
#include <vector>
#include "assert.h"

BEGIN_JLIB_NAMESPACE

//#define JLIB_USECUSTOMVECTOR

#ifndef JLIB_USECUSTOMVECTOR
using std::vector;
#else
template <typename T> class vector
    {
public:
    T &operator[]( unsigned int x )
        {
        assert( x < size() );
        return _start[x];
        }
    T &at( unsigned int x )
        {
        assert( x < size() );
        return _start[x];
        }
    void push_back( T x )
        {
        ensureAllocated( size() + 1 );
        back() = x;
        }
    void push_front( T x )
        {
        ensureAllocated( size() + 1 );
        budgeAll( 1 );
        front() = x;
        }
    T *begin()
        {
        assert( _start != _used );
        return _start;
        }
    T *end();
    T &front();
    T &back();
    T pop_front( );
    T pop_back( );
    void erase( T * );
    void erase( unsigned int );
    unsigned int size()
        {
        return ( _used - _start ) / sizeof( T );
        }
    void resize( unsigned int x )
        {
        int diff = x - size();
        if( diff < 0 )
            {
            _used = _start + ( diff * sizeof( T ) );
            }
        else if ( diff > 0 )
            {
            ensureAllocated( x );
            }
        }
    void insert( T *, T );
    void insert( unsigned int, T );
    void clear()
        {
        _used = _start;
        }
private:
    void ensureAllocated( unsigned int );
    void budgeAll( int );
    T *_start;
    T *_used;
    T *_end;
    };
#endif

END_JLIB_NAMESPACE

#endif
